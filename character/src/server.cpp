#include "server.hpp"

thread_local std::unique_ptr<ares::database::db> ares::character::server::db = nullptr;

ares::character::server::server(std::shared_ptr<spdlog::logger> log,
                                std::shared_ptr<asio::io_context> io_context,
                                const config& conf) :
  ares::network::server<server, session>(log, io_context, *conf.network_threads),
  conf_(conf),
  auth_requests(std::make_shared<auth_request_manager>(*this, std::chrono::seconds{5})),
  maps(std::make_unique<maps_manager>(log_, conf_.postgres, conf_.zone_servers, conf_.grfs)) {
  std::shared_ptr<ares::grf::resource_set> resources;
  std::set<std::string> used_maps;
  std::vector<std::string> unknown_maps;
  std::vector<std::string> duplicate_maps;

  std::string msg;
  if (unknown_maps.size() > 0) {
    msg = "Unknown map names: ";
    bool need_comma = false;
    for (const auto& m : unknown_maps) {
      if (need_comma) msg += ", ";
      msg += m;
      need_comma = true;
    }
    msg += ". ";
  }
  if (duplicate_maps.size() > 0) {
    msg = msg + "Maps assigned to more than one zone server: ";
    bool need_comma = false;
    for (const auto& m : unknown_maps) {
      if (need_comma) msg += ", ";
      msg += m;
    }
    msg += ".";
  }

  if (msg.size() > 0) {
    log_->error("Error in maps configuration. " + msg);
    throw std::runtime_error("Error in maps configuration. " + msg);
  }
}

void ares::character::server::init_thread_local() {
  db = std::make_unique<ares::database::db>(log_, conf_.postgres->dbname, conf_.postgres->host, conf_.postgres->port, conf_.postgres->user, conf_.postgres->password);
}

void ares::character::server::start() {
  if (conf_.listen_ipv4.size() > 0) {
    for (const auto& listen : conf_.listen_ipv4) {
      log_->info("starting listener at {}:{}",
                 listen.address().to_v4().to_string(),
                 listen.port());
      ares::network::server<server, session>::start(listen);
    }

    auto& ep = conf_.account_server->connect;
    auto account_server = std::make_shared<session>(*this,
                                                    ep,
                                                    nullptr,
                                                    std::chrono::seconds{30});
    account_server->variant().emplace<account_server::state>(*this, *account_server);
    account_server->set_reconnect_timer(std::chrono::seconds{0}, std::chrono::seconds{5});
    account_server_ = account_server;
  } else {
    log_->error("Can't start: listen ipv4 configuration is empty");
  }
}

void ares::character::server::create_session(std::shared_ptr<asio::ip::tcp::socket> socket) {
  SPDLOG_TRACE(log_, "character::server::create_session");
  auto s = std::make_shared<session>(*this, std::optional<asio::ip::tcp::endpoint>{}, socket, std::chrono::seconds{120});
  add(s);
  s->receive();
  s->reset_idle_timer();
}

auto ares::character::server::conf() const -> const config& {
  return conf_;
}

void ares::character::server::add(std::shared_ptr<session> s) {
  struct visitor {
    visitor(server& serv, std::shared_ptr<session> s) :
      serv_(serv), s_(s) {};
    
    void operator()(const mono::state&) {
      serv_.mono_.insert(s_);
    }

    void operator()(const zone_server::state&) {
      serv_.zone_servers_.insert(s_);
      serv_.mono_.erase(s_);
    }

    void operator()(const client::state&) {
      serv_.clients_.insert({s_->as_client().account_id, s_});
      serv_.mono_.erase(s_);
    }

    void operator()(const account_server::state&) {
      serv_.account_server_ = s_;
    }
  private:
    server& serv_;
    std::shared_ptr<session> s_;
  };

  std::visit(visitor(*this, s), s->variant());
}

void ares::character::server::remove(std::shared_ptr<session> s) {
  struct visitor {
    
    visitor(server& serv, const std::weak_ptr<session>& s) :
      serv_(serv), s_(s) {};

    void operator()(const mono::state&) {
      if (serv_.auth_requests) {
        serv_.auth_requests->cancel(s_);
      }
      serv_.mono_.erase(s_);
    }

    void operator()(const zone_server::state&) {
      for (auto it = serv_.account_id_to_zone_server_.begin(); it != serv_.account_id_to_zone_server_.end();) {
        const auto& r = *it;
        auto sp = r.second.lock();
        if (!sp || (sp == s_)) {
          // Kill client if zone_server disconnects?
          // clients.erase(r.first);
          it = serv_.account_id_to_zone_server_.erase(it);
        } else {
          ++it;
        }
      }
      serv_.maps->remove_zone_session(s_);
      serv_.zone_servers_.erase(s_);
    }

    void operator()(const client::state&) {
      serv_.account_id_to_zone_server_.erase(s_->as_client().account_id);
      serv_.clients_.erase(s_->as_client().account_id);
    }

    void operator()(const account_server::state&) {
      // weak_ptr will get deleted anyway
    }
  private:
    server& serv_;
    std::shared_ptr<session> s_;
  };

  std::visit(visitor(*this, s), s->variant());
}

auto ares::character::server::account_server() const -> std::shared_ptr<session> {
  auto s = account_server_.lock();
  if (!s) log_->error("Requested access to account server which is currently null");
  return s;
}

auto ares::character::server::zone_server_by_login(const std::string& login) const -> std::shared_ptr<session> {
  auto found = std::find_if(zone_servers_.begin(), zone_servers_.end(), [&login] (const std::weak_ptr<session>& s) {
      return s.lock()->as_zone_server().login == login;
    });
  if (found != zone_servers_.end()) {
    if (auto s = found->lock()) return s;
  }
  return nullptr;
}

auto ares::character::server::find_client_session(const model::account_id& account_id) const -> std::shared_ptr<session> {
  auto found = clients_.find(account_id);
  if (found != clients_.end()) {
    return found->second.lock();
  }
  return nullptr;
}

void ares::character::server::link_to_zone_session(const model::account_id& account_id, std::shared_ptr<session> s) {
  account_id_to_zone_server_[account_id] = s;
}

void ares::character::server::unlink_from_zone_session(const model::account_id& account_id, std::shared_ptr<session> s) {
  auto found = account_id_to_zone_server_.find(account_id);
  if (found != account_id_to_zone_server_.end()) {
    auto ptr = found->second.lock();
    if (!ptr || (ptr == s)) {
      account_id_to_zone_server_.erase(account_id);
    } else {
      log_->warn("unlink_from_zone_session account_id is linked to another zone server session, not unlinking");
    }
  }
}
