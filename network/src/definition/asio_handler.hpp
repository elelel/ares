#pragma once

#include "../asio_handler"

template <typename Handler, typename Session>
inline ares::network::asio_handler<Handler, Session>::asio_handler(const std::shared_ptr<Session> session) :
  session_(session) {
}

template <typename Handler, typename Session>
inline std::shared_ptr<spdlog::logger>& ares::network::asio_handler<Handler, Session>::log() {
  return session_->log_;
}
