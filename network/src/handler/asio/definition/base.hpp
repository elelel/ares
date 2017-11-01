#pragma once

#include "../base.hpp"

template <typename Handler, typename Session>
inline ares::network::handler::asio::base<Handler, Session>::base(const std::shared_ptr<Session>& session) :
  session_(session) {
}

template <typename Handler, typename Session>
inline std::shared_ptr<spdlog::logger>& ares::network::handler::asio::base<Handler, Session>::log() {
  return session_->log_;
}

