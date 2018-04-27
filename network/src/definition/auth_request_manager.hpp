#pragma once

#include "../auth_request_manager.hpp"

template <typename Derived, typename Server, typename Session>
inline ares::network::auth_request_manager<Derived, Server, Session>::auth_request_manager(Server& server,
                                                                                           const std::chrono::seconds auth_timeout) :
  server_(server),
  i_{0},
  auth_timeout_(auth_timeout) {
  }

template <typename Derived, typename Server, typename Session>
inline void ares::network::auth_request_manager<Derived, Server, Session>::defuse_asio() {
  for (const auto& p : pending_) {
    p.second.timeout_timer.cancel();
  }
  pending_.clear();
}

template <typename Derived, typename Server, typename Session>
inline uint32_t ares::network::auth_request_manager<Derived, Server, Session>::new_request(std::shared_ptr<Session> s) {
  ++i_;
  auto timer = std::make_shared<asio::steady_timer>(*server_.io_context());
  pending_[i_] = {s, timer};
  timer->expires_at(std::chrono::steady_clock::now() + auth_timeout_);
  auto m = static_cast<Derived*>(this)->shared_from_this();
  timer->async_wait([i = i_, m, s] (const std::error_code& ec) {
      if (ec.value() == 0) {
        SPDLOG_TRACE(s->log(), "Auth timeout timer for session {} fired", (void*)s.get());
        m->auth_timeout(s);
        m->pending_.erase(i);
      }
    });
  return i_;
}

template <typename Derived, typename Server, typename Session>
inline auto ares::network::auth_request_manager<Derived, Server, Session>::responded(const uint32_t request_id) -> std::shared_ptr<Session> {
  auto found = pending_.find(request_id);
  std::shared_ptr<Session> s;
  if (found != pending_.end()) {
    found->second.timer->cancel();
    s = found->second.session.lock();
    pending_.erase(found);
  };
  return s;
}

template <typename Derived, typename Server, typename Session>
inline void ares::network::auth_request_manager<Derived, Server, Session>::cancel(std::shared_ptr<Session> s) {
  auto found = std::find_if(pending_.begin(), pending_.end(), [s] (const auto& r) {
      return r.second.session.lock() == s;
    });
  if (found != pending_.end()) {
    found->second.timer->cancel();
    pending_.erase(found);
    cancel(s);
  };
}

template <typename Derived, typename Server, typename Session>
inline std::mutex& ares::network::auth_request_manager<Derived, Server, Session>::mutex() {
  return mutex_;
}
