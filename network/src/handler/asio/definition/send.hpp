#pragma once

#include "../send.hpp"

template <typename Handler, typename Session>
inline void ares::network::handler::asio::send<Handler, Session>::operator()(const std::error_code& ec, size_t sent_sz) {
  if (ec.value() == 0) {
    SPDLOG_TRACE(this->log(), "ares::send_handler acquiring send lock");
    std::lock_guard lock(mutex());
    SPDLOG_TRACE(this->log(), "ares::send_handler send lock acquired");
    buf().pop_front(sent_sz);
    SPDLOG_TRACE(this->log(), "ares::send_handler removed {} bytes from send buf", sent_sz);
    this->session_->reset_inactivity_timer();
    if (buf().size() > 0) {
      SPDLOG_TRACE(this->log(), "Still {} bytes left to send", buf().size());
      this->session_->socket_->async_write_some(::asio::buffer(buf().begin(), buf().head_size()), this->session_->make_send_handler());
    } else {
      SPDLOG_TRACE(this->log(), "All bytes sent");
      this->session_->sending_ = false;
    }
  } else {
    switch (ec.value()) {
    case ::asio::error::operation_aborted:
      static_cast<Handler&>(*this).on_operation_aborted();
      break;
    case ::asio::error::connection_reset:
      this->session_->connected_ = false;
      static_cast<Handler&>(*this).on_connection_reset();
      break;
    case ::asio::error::eof:
      this->session_->connected_ = false;
      static_cast<Handler&>(*this).on_eof();
      break;
    default:
      this->log()->error("send failed on socket {0:#x}, error {}, {}", (uintptr_t)this->session_->socket_.get(), ec.value(), ec.message());
      static_cast<Handler&>(*this).on_socket_error();
      break;
    }
  }
  SPDLOG_TRACE(this->log(), "ares::send_handler::operator() end");
}

template <typename Handler, typename Session>
inline elelel::network_buffer& ares::network::handler::asio::send<Handler, Session>::buf() {
  return this->session_->send_buf_;
}

template <typename Handler, typename Session>
inline std::mutex& ares::network::handler::asio::send<Handler, Session>::mutex() {
  return this->session_->send_mutex_;
}
