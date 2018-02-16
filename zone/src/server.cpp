#include "server.hpp"

#include "session.hpp"

ares::zone::server::server(std::shared_ptr<spdlog::logger> log,
                           std::shared_ptr<asio::io_context> io_context,
                           const ares::zone::config& conf) :
  ares::network::server<server, session>(log, io_context, *conf.network_threads),
  conf_(conf),
  //  world_(*this),
  db(log, *conf.postgres) {
  log_->set_level(spdlog::level::trace);
  //  world::initialize(world_);
}

void ares::zone::server::start() {
  if (conf_.listen_ipv4.size() > 0) {
    for (const auto& listen : conf_.listen_ipv4) {
      ares::network::server<server, session>::start(listen);
    }
    log()->info("Establishing connection to character server");
    auto& ep = conf_.character_server->connect;
    char_server_ = std::make_shared<session>(*this,
                                             ep,
                                             nullptr,
                                             std::chrono::seconds{30});
    char_server_->variant().emplace<character_server::state>(*this, *char_server_);
    char_server_->set_reconnect_timer(std::chrono::seconds{0}, std::chrono::seconds{5});
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

auto ares::zone::server::char_server() const -> const session_ptr& {
  return char_server_;
}

void ares::zone::server::add(session_ptr s) {
  struct visitor {
    visitor(server& serv, const session_ptr& s) :
      serv(serv), s(s) {};

    void operator()(const mono::state&) {
      serv.mono_.insert(s);
    }

    void operator()(const client::state&) {
      serv.clients_.insert({s->as_client().aid, s});
      serv.mono_.erase(s);
    }

    void operator()(const character_server::state&) {
      serv.char_server_ = s;
    }
    
  private:
    server& serv;
    session_ptr s;
  };
  std::visit(visitor(*this, s), s->variant());
}

void ares::zone::server::remove(session_ptr s) {
  struct visitor {
    visitor(server& serv, const session_ptr& s) :
      serv(serv), s(s) {};

    void operator()(const mono::state&) {
      serv.mono_.erase(s);
    }

    void operator()(const client::state&) {
      serv.clients_.erase(s->as_client().aid);
    }

    void operator()(const character_server::state&) {
      if (s == serv.char_server_) serv.char_server_ == nullptr;
    }
  private:
    server& serv;
    session_ptr s;
  };
  std::visit(visitor(*this, s), s->variant());
}


auto ares::zone::server::client_by_aid(const uint32_t aid) -> session_ptr {
  auto found = clients_.find(aid);
  if (found != clients_.end()) {
    return found->second;
  } else {
    return nullptr;
  }
}
