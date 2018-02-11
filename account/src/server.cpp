#include "server.hpp"

#include "session.hpp"

ares::account::server::server(std::shared_ptr<spdlog::logger> log,
                              std::shared_ptr<asio::io_context> io_context,
                              const ares::account::config& conf) :
  ares::network::server<server, session>(log, io_context, *conf.network_threads),
  conf_(conf),
  db(log, *conf.postgres) {
  log_->set_level(spdlog::level::trace);
}

void ares::account::server::start() {
  if (conf_.listen_ipv4.size() > 0) {
    for (const auto& listen : conf_.listen_ipv4) {
      ares::network::server<server, session>::start(listen);
    }
  } else {
    log()->error("Can't start: listen ipv4 configuration is empty");
  }
}

void ares::account::server::create_session(std::shared_ptr<asio::ip::tcp::socket> socket) {
  SPDLOG_TRACE(log_, "account::server::create_session");
  auto s = std::make_shared<session>(*this, std::optional<asio::ip::tcp::endpoint>{}, socket, std::chrono::seconds{120});
  mono_.insert(s);
  s->receive();
  s->reset_idle_timer();      
}

auto ares::account::server::conf() const -> const config& {
  return conf_;
}

void ares::account::server::add(session_ptr s) {
  struct visitor {
    visitor(server& serv, const session_ptr& s) :
      serv(serv), s(s) {};

    void operator()(const mono::state&) {
      serv.mono_.insert(s);
    }

    void operator()(const client::state&) {
      SPDLOG_TRACE(s->log(), "Adding session as client, AID {}", s->as_client().aid);
      serv.clients_.insert({s->as_client().aid, s});
      SPDLOG_TRACE(s->log(), "Clients list has now {} elements", serv.clients_.size());
      serv.mono_.erase(s);
    }

    void operator()(const character_server::state&) {
      serv.char_servers_.insert(s);
      serv.mono_.erase(s);
    }
    
  private:
    server& serv;
    session_ptr s;
  };
  std::visit(visitor(*this, s), s->variant());
}

void ares::account::server::remove(session_ptr s) {
  struct visitor {
    visitor(server& serv, const session_ptr& s) :
      serv(serv), s(s) {};

    void operator()(const mono::state&) {
      serv.mono_.erase(s);
    }

    void operator()(const client::state&) {
      SPDLOG_TRACE(s->log(), "Removing client session, AID {}", s->as_client().aid);
      serv.clients_.erase(s->as_client().aid);
    }

    void operator()(const character_server::state&) {
      serv.char_servers_.erase(s);
    }
  private:
    server& serv;
    session_ptr s;
  };
  std::visit(visitor(*this, s), s->variant());
}

auto ares::account::server::client_by_aid(const uint32_t aid) -> session_ptr {
  SPDLOG_TRACE(log(), "Searching in client list of size {} aid {}", clients_.size(), aid);
  auto found = clients_.find(aid);
  if (found != clients_.end()) {
    return found->second;
  } else {
    return nullptr;
  }
}


void ares::account::server::link_aid_to_char_server(const uint32_t aid, session_ptr s) {
  aid_to_char_server_[aid] = s;
}

void ares::account::server::unlink_aid_from_char_server(const uint32_t aid, session_ptr s) {
  auto found = aid_to_char_server_.find(aid);
  if (found != aid_to_char_server_.end()) {
    if (found->second == s) {
      aid_to_char_server_.erase(aid);
    } else {
      log_->warn("unlink_aid_from_char_server aid is linked to another char server session, not unlinking");
    }
  }
}

auto ares::account::server::char_servers() const -> const std::set<session_ptr>& {
  return char_servers_;
}

