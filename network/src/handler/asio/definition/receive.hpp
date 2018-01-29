#pragma once

#include "../receive.hpp"

template <typename Handler, typename Session>
inline void ares::network::handler::asio::receive<Handler, Session>::operator()(const std::error_code& ec, const size_t received_sz) {
  SPDLOG_TRACE(this->log(), "handler::asio::receive begin, ec = {}, received_sz = {}", ec.value(), received_sz);
  if (ec.value() == 0) {
    try {
      SPDLOG_TRACE(this->log(), "handler::asio::receive acquiring lock");
      {
        std::unique_lock<std::mutex> lock(mutex());
        SPDLOG_TRACE(this->log(), "handler::asio::receive lock acquired");
        if (received_sz > 0) {
          buf().adjust_size_by(received_sz);
          SPDLOG_TRACE(this->log(), "resetting inactivity timer");
          this->session_->reset_inactivity_timer();

          size_t need_more{0};
          while ((buf().size() > 0) && (need_more == 0)) {
            if (buf().size() >= 2) {
              // Need at least 2 bytes in buffer to dispatch by packet id
              if (buf().head_size() < 2) buf().defragment();
              auto PacketType = (uint16_t*)buf().begin();
              SPDLOG_TRACE(this->log(), "receive handler calling dispatch, buf().size() = {}, PacketType = {:#x}", buf().size(), *PacketType);
              need_more = static_cast<Handler&>(*this).dispatch(*PacketType);
            } else {
              need_more = 2 - buf().size();
            }
          }
          lock.unlock();
          this->session_->receiving_ = false;
          if (need_more > 0) {
            // Receive the number of bytes needed reported by packet handler
            this->session_->receive(need_more);
          } else {
            // Wait till new packet id in buffer
            this->session_->receive(2);
          }
        } else {
          SPDLOG_TRACE(this->log(), "received zero bytes, terminating session!");
          static_cast<Handler&>(*this).terminate_session();      
        }
      }
    } catch (terminate_session) {
      SPDLOG_TRACE(this->log(), "Dispatcher signaled terminate_session");
      static_cast<Handler&>(*this).terminate_session();      
    }
  } else {
    this->session_->receiving_ = false;
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
      this->log()->error("receive failed on socket {0:#x}, error {}, {}", (uintptr_t)this->session_->socket_.get(), ec.value(), ec.message());
      static_cast<Handler&>(*this).on_socket_error();
      break;
    }
  }
}


template <typename Handler, typename Session>
inline elelel::network_buffer& ares::network::handler::asio::receive<Handler, Session>::buf() {
  return this->session_->recv_buf_;
}

template <typename Handler, typename Session>
inline std::mutex& ares::network::handler::asio::receive<Handler, Session>::mutex() {
  return this->session_->recv_mutex_;
}
