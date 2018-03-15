#include "server.hpp"

#include "session.hpp"

thread_local std::shared_ptr<ares::database::db> ares::zone::server::db = nullptr;

ares::zone::server::server(std::shared_ptr<spdlog::logger> log,
                           std::shared_ptr<asio::io_context> io_context,
                           const ares::zone::config& conf) :
  ares::network::server<server, session>(log, io_context, *conf.network_threads),
  auth_requests(std::make_shared<auth_request_manager>(*this, std::chrono::seconds(5))),
  maps(log, conf.postgres),
  pcs(log),
  conf_(conf) {
}

void ares::zone::server::init_thread_local() {
  db = std::make_shared<ares::database::db>(log_, conf_.postgres->dbname, conf_.postgres->host, conf_.postgres->port, conf_.postgres->user, conf_.postgres->password);
}

void ares::zone::server::start() {
  if (conf_.listen_ipv4.size() > 0) {
    for (const auto& listen : conf_.listen_ipv4) {
      ares::network::server<server, session>::start(listen);
    }
    log()->info("Establishing connection to character server");
    auto& ep = conf_.character_server->connect;
    auto char_server = std::make_shared<session>(*this,
                                                 ep,
                                                 nullptr,
                                                 std::chrono::seconds{30});
    char_server->variant().emplace<character_server::state>(*this, *char_server);
    char_server->set_reconnect_timer(std::chrono::seconds{0}, std::chrono::seconds{5});
    char_server_ = char_server;
  } else {
    log()->error("Can't start: listen ipv4 configuration is empty");
  }
}

void ares::zone::server::create_session(std::shared_ptr<asio::ip::tcp::socket> socket) {
  SPDLOG_TRACE(log_, "zone::server::create_session");
  auto s = std::make_shared<session>(*this, std::optional<asio::ip::tcp::endpoint>{}, socket, std::chrono::seconds{120});
  add(s);
  s->receive();
  s->reset_idle_timer();      
}

auto ares::zone::server::conf() const -> const config& {
  return conf_;
}

auto ares::zone::server::char_server() const -> std::shared_ptr<session> {
  return char_server_.lock();
}

void ares::zone::server::add(std::shared_ptr<session> s) {
  struct visitor {
    visitor(server& serv, const std::shared_ptr<session>& s) :
      serv_(serv), s_(s) {};

    void operator()(const mono::state&) {
      if (serv_.auth_requests) {
        serv_.auth_requests->cancel(s_);
      }
      serv_.mono_.insert(s_);
    }

    void operator()(const client::state&) {
      if (serv_.auth_requests) {
        serv_.auth_requests->cancel(s_);
      }
      serv_.clients_.insert({s_->as_client().account_id, s_});
      serv_.mono_.erase(s_);
    }

    void operator()(const character_server::state&) {
      serv_.char_server_ = s_;
    }
    
  private:
    server& serv_;
    std::shared_ptr<session> s_;
  };
  std::visit(visitor(*this, s), s->variant());
}

void ares::zone::server::remove(std::shared_ptr<session> s) {
  struct visitor {
    visitor(server& serv, const std::shared_ptr<session>& s) :
      serv_(serv), s_(s) {};

    void operator()(const mono::state&) {
      serv_.mono_.erase(s_);
    }

    void operator()(const client::state&) {
      serv_.clients_.erase(s_->as_client().account_id);
    }

    void operator()(const character_server::state&) {
    }
  private:
    server& serv_;
    std::shared_ptr<session> s_;
  };
  
  std::visit(visitor(*this, s), s->variant());
}

auto ares::zone::server::find_client_session(const model::account_id& account_id) const -> std::shared_ptr<session> {
  auto found = clients_.find(account_id);
  if (found != clients_.end()) {
    if (auto s = found->second.lock()) return s;
  }
  return nullptr;
}
