#include "server.hpp"

#include "state.hpp"

ares::character::server::server(character::state& server_state) :
  ares::network::server<server>(server_state.log(), server_state.io_service(), *server_state.conf.network_threads),
  state_(server_state) {
}

void ares::character::server::start() {
  if (state_.conf.listen_ipv4.size() > 0) {
    for (const auto& listen : state_.conf.listen_ipv4) {
      log_->info("starting listener at {}:{}",
                 listen.address().to_v4().to_string(),
                 listen.port());
      ares::network::server<server>::start(listen);
    }
    if (state_.log() != nullptr) {
      account_server_ = std::make_shared<session>(state_, std::shared_ptr<asio::ip::tcp::socket>{});
      account_server_->variant().emplace<account_server::state>(state_, *account_server_);
      account_server_->as_account_server().reconnect_timer.fire();
    } else {
      throw std::runtime_error("server::start() failure - state_.log() is nullptr");
    }
  } else {
    log_->error("Can't start: listen ipv4 configuration is empty");
  }
}

void ares::character::server::create_session(std::shared_ptr<asio::ip::tcp::socket> socket) {
  SPDLOG_TRACE(log_, "character::server::create_session");
  auto s = std::make_shared<session>(state_, socket);
  add(s);
  s->reset_inactivity_timer();
  s->receive();
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
      decltype(auth_aid_requests_)::iterator found;
      while ((found =
              std::find_if(serv.auth_aid_requests_.begin(), serv.auth_aid_requests_.end(), [this] (auto p) {
                  return p.second.first == s;
                })) != serv.auth_aid_requests_.end()) {
        serv.auth_aid_requests_.erase(found);
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

auto ares::character::server::account_server() const -> const session_ptr& {
  return account_server_;
}

auto ares::character::server::client_by_aid(const uint32_t aid) -> session_ptr {
  auto found = clients_.find(aid);
  if (found != clients_.end()) {
    return found->second;
  } else {
    return nullptr;
  }
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
 
void ares::character::server::add_auth_aid_request(const int32_t request_id, session_ptr s) {
  prune_auth_aid_requests();
  auto now = std::chrono::steady_clock::now();
  auth_aid_requests_.insert({request_id, {s, now}});
}

auto ares::character::server::session_by_auth_request_id(const int32_t request_id) -> session_ptr {
  prune_auth_aid_requests();
  auto found = auth_aid_requests_.find(request_id);
  if (found != auth_aid_requests_.end()) return found->second.first;
  return nullptr;
}

void ares::character::server::remove_auth_aid_request(const int32_t request_id) {
  auth_aid_requests_.erase(request_id);
}

void ares::character::server::prune_auth_aid_requests() {
  auto now = std::chrono::steady_clock::now();
  std::vector<session_ptr> timed_out;
  for (const auto& p : auth_aid_requests_) {
    if (now - p.second.second > std::chrono::seconds{60}) {
      timed_out.push_back(p.second.first);
    }
  }
  for (auto s : timed_out) remove(s);
}
