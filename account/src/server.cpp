#include "server.hpp"

#include "state.hpp"

ares::account::server::server(account::state& server_state) :
  ares::network::server<server>(server_state.log(),
                                server_state.io_service(),
                                *server_state.conf.network_threads),
  state_(server_state) {
}

void ares::account::server::start() {
  if (state_.conf.listen_ipv4.size() > 0) {
    for (const auto& listen : state_.conf.listen_ipv4) {
      ares::network::server<server>::start(listen);
    }
  } else {
    log_->error("Can't start: listen ipv4 configuration is empty");
  }
}

void ares::account::server::create_session(std::shared_ptr<asio::ip::tcp::socket> socket) {
  SPDLOG_TRACE(log_, "account::server::create_session");
  auto s = std::make_shared<session>(state_, socket);
  s->reset_inactivity_timer();
  mono_.insert(s);
  s->receive();
}

void ares::account::server::add(session_ptr s) {
  struct visitor {
    visitor(server& serv, const session_ptr& s) :
      serv(serv), s(s) {};

    void operator()(const mono::state&) {
      serv.mono_.insert(s);
    }
    
    void operator()(const char_server::state&) {
      serv.char_servers_.insert(s);
      serv.mono_.erase(s);
    }

    void operator()(const client::state&) {
      serv.clients_.insert({s->as_client().aid, s});
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

    void operator()(const char_server::state&) {
      for (auto it = serv.aid_to_char_server_.begin(); it != serv.aid_to_char_server_.end();) {
        const auto& r = *it;
        if ((r.second == s) || (r.second == nullptr)) {
          it = serv.aid_to_char_server_.erase(it);
        } else {
          ++it;
        }
      }
      serv.char_servers_.erase(s);
    }

    void operator()(const client::state&) {
      serv.aid_to_char_server_.erase(s->as_client().aid);
      serv.clients_.erase(s->as_client().aid);
    }
  private:
    server& serv;
    session_ptr s;
  };
  std::visit(visitor(*this, s), s->variant());
}

auto ares::account::server::char_servers() const -> const std::set<session_ptr>& {
  return char_servers_;
}

auto ares::account::server::client_by_aid(const uint32_t aid) -> session_ptr {
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
 
