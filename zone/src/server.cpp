#include "server.hpp"

#include "state.hpp"

ares::zone::server::server(state& zone_state) :
  ares::network::server<server>(zone_state.log(),
                                zone_state.io_service(),
                                *zone_state.conf.network_threads),
  state_(zone_state) {
}

void ares::zone::server::start() {
  if (state_.conf.listen_ipv4.size() > 0) {
    for (const auto& listen : state_.conf.listen_ipv4) {
      ares::network::server<server>::start(listen);
    }
    char_server_ = std::make_shared<session>(state_, nullptr);
    char_server_->variant().emplace<character_server::state>(state_, *char_server_);
    char_server_->as_character_server().reconnect_timer.fire();
  } else {
    log_->error("Can't start: listen ipv4 configuration is empty");
  }
}

void ares::zone::server::create_session(std::shared_ptr<boost::asio::ip::tcp::socket> socket) {
  SPDLOG_TRACE(log_, "zone::server::create_session");
  auto s = std::make_shared<session>(state_, socket);
  s->reset_inactivity_timer();
  mono_.insert(s);
  s->receive();
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

auto ares::zone::server::char_server() const -> const session_ptr& {
  return char_server_;
}

