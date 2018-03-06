#include "server.hpp"

ares::character::server::server(std::shared_ptr<spdlog::logger> log,
                                std::shared_ptr<asio::io_context> io_context,
                                const config& conf) :
  ares::network::server<server, session>(log, io_context, *conf.network_threads),
  conf_(conf),
  db(log, conf.postgres->dbname, conf.postgres->host, conf.postgres->port, conf.postgres->user, conf.postgres->password),
  auth_requests(std::make_shared<auth_request_manager>(*this, std::chrono::seconds{5})),
  maps(std::make_shared<maps_manager>(*this)) {


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

void ares::character::server::start() {
  if (conf_.listen_ipv4.size() > 0) {
    for (const auto& listen : conf_.listen_ipv4) {
      log_->info("starting listener at {}:{}",
                 listen.address().to_v4().to_string(),
                 listen.port());
      ares::network::server<server, session>::start(listen);
    }

    auto& ep = conf_.account_server->connect;
    account_server_ = std::make_shared<session>(*this,
                                                ep,
                                                nullptr,
                                                std::chrono::seconds{30});
    account_server_->variant().emplace<account_server::state>(*this, *account_server_);
    account_server_->set_reconnect_timer(std::chrono::seconds{0}, std::chrono::seconds{5});

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

void ares::character::server::add(session_ptr s) {
  struct visitor {
    visitor(server& serv, const session_ptr& s) :
      serv(serv), s(s) {};
    
    void operator()(const mono::state&) {
      serv.mono_.insert(s);
    }

    void operator()(const zone_server::state&) {
      serv.zone_servers_.insert(s);
      serv.mono_.erase(s);
    }

    void operator()(const client::state&) {
      serv.clients_.insert({s->as_client().aid, s});
      serv.mono_.erase(s);
    }

    void operator()(const account_server::state&) {
      serv.account_server_ = s;
    }
  private:
    server& serv;
    session_ptr s;
  };
  std::visit(visitor(*this, s), s->variant());
}

void ares::character::server::remove(session_ptr s) {
  struct visitor {
    visitor(server& serv, const session_ptr& s) :
      serv(serv), s(s) {};

    void operator()(const mono::state&) {
      if (serv.auth_requests) {
        serv.auth_requests->cancel(s);
      }
      serv.mono_.erase(s);
    }

    void operator()(const zone_server::state&) {
      for (auto it = serv.aid_to_zone_server_.begin(); it != serv.aid_to_zone_server_.end();) {
        const auto& r = *it;
        if ((r.second == s) || (r.second == nullptr)) {
          // Kill client if zone_server disconnects?
          // clients.erase(r.first);
          it = serv.aid_to_zone_server_.erase(it);
        } else {
          ++it;
        }
      }
      serv.maps->remove_zone_session(s);
      serv.zone_servers_.erase(s);
    }

    void operator()(const client::state&) {
      serv.aid_to_zone_server_.erase(s->as_client().aid);
      serv.clients_.erase(s->as_client().aid);
    }

    void operator()(const account_server::state&) {
      if (s == serv.account_server_) serv.account_server_ == nullptr;
    }
  private:
    server& serv;
    session_ptr s;
  };
  
  std::visit(visitor(*this, s), s->variant());
}

auto ares::character::server::zone_servers() const -> const std::set<session_ptr>& {
  return zone_servers_;
}

auto ares::character::server::zone_server_by_map_id(const uint32_t map_id) const -> session_ptr {
  auto found = map_id_to_zone_server_.find(map_id);
  if (found != map_id_to_zone_server_.end()) {
    return found->second;
  }
  return nullptr;
}

auto ares::character::server::account_server() const -> const session_ptr& {
  return account_server_;
}

auto ares::character::server::client_by_aid(const uint32_t aid) -> session_ptr {
  auto found = clients_.find(aid);
  if (found != clients_.end()) {
    return found->second;
  }
  return nullptr;
}

auto ares::character::server::clients() const -> const std::map<uint32_t, session_ptr>& {
  return clients_;
}

void ares::character::server::link_aid_to_zone_server(const uint32_t aid, session_ptr s) {
  aid_to_zone_server_[aid] = s;
}

void ares::character::server::unlink_aid_from_zone_server(const uint32_t aid, session_ptr s) {
  auto found = aid_to_zone_server_.find(aid);
  if (found != aid_to_zone_server_.end()) {
    if (found->second == s) {
      aid_to_zone_server_.erase(aid);
    } else {
      log_->warn("unlink_aid_from_zone_server aid is linked to another zone server session, not unlinking");
    }
  }
}
