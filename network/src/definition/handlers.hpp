#pragma once

#include "../handlers.hpp"

template <typename Session>
inline ares::network::reconnect_timer_handler<Session>::reconnect_timer_handler(std::shared_ptr<Session> s,
                                                                                const std::chrono::seconds timeout) :
  s_(s),
  timeout_(timeout) {
  };

template <typename Session>
inline ares::network::reconnect_timer_handler<Session>::reconnect_timer_handler(const reconnect_timer_handler& other) :
  s_(other.s_),
  timeout_(other.timeout_) {
}

template <typename Session>
inline void ares::network::reconnect_timer_handler<Session>::operator()(const std::error_code& ec) {
  std::stringstream ss;
  ss << *s_->connect_ep_;
  if (ec.value() == 0) {
    if (!s_->connecting_ && !s_->connected_) {
      s_->connecting_ = true;
      s_->log()->info("Establishing connection with {}", ss.str());
      SPDLOG_TRACE(s_->log(), "Locking send mutex to clear buffer");
      {
        std::lock_guard<std::mutex> lock(s_->send_mutex_);
        s_->send_buf_.clear();
      }
      SPDLOG_TRACE(s_->log(), "Locking recv mutex to clear buffer");
      {
        std::lock_guard<std::mutex> lock(s_->recv_mutex_);
        s_->recv_buf_.clear();
      }
      s_->socket_ = std::make_shared<asio::ip::tcp::socket>(*s_->io_context());
      s_->idle_timer_->cancel();
      std::error_code ec;
      s_->socket_->open(s_->connect_ep_->protocol(), ec);
      if (ec.value() == 0) {
        auto s = this->s_;
        s_->socket_->async_connect(*s_->connect_ep_, [s] (const std::error_code& ec) {
            s->connecting_ = false;
            s->receiving_ = false;
            s->sending_ = false;
            std::stringstream ss;
            ss << *s->connect_ep_;
            if (ec.value() == 0) {
              s->connected_ = true;
              s->log()->info("Connection with {} established", ss.str());
              s->reset_idle_timer();
              static_cast<Session&>(*s).on_connect();
              s->receive();
            } else {
              s->log()->warn("Connection to {} failed while connecting the socket with error {}, {}", ss.str(), ec.value(), ec.message());
              s->set_reconnect_timer(s->reconnect_timer_timeout_, s->reconnect_timer_timeout_);
            }
          });
      } else {
        s_->log()->warn("Connection to {} failed while opening the socket with error {}, {}", ss.str(), ec.value(), ec.message());
        s_->set_reconnect_timer(s_->reconnect_timer_timeout_, s_->reconnect_timer_timeout_);
      }
    } else {
      s_->log()->warn("Connection to {} is still established or in process of being established, ignoring reconnection request", ss.str());
    }
  } else {
    SPDLOG_TRACE(s_->log(), "Reconnect timer error {}, {}", ec.value(), ec.message());
  }
}

template <typename Session>
inline ares::network::send_handler<Session>::send_handler(std::shared_ptr<Session> s) :
  s_(s) {
}

template <typename Session>
inline ares::network::send_handler<Session>::send_handler(const send_handler& other) :
  s_(other.s_) {
}

template <typename Session>
inline void ares::network::send_handler<Session>::operator()(const std::error_code& ec, const size_t sent_sz) {
  if (ec.value() == 0) {
    SPDLOG_TRACE(s_->log(), "send handler ackquiring lock");
    std::lock_guard lock(s_->send_mutex_);
    auto& buf = s_->send_buf_;
    buf.pop_front(sent_sz);
    SPDLOG_TRACE(s_->log(), "send handler removed {} bytes from send buf", sent_sz);
    if (buf.size() > 0) {
      SPDLOG_TRACE(s_->log(), "send handler still {} bytes left to send", buf.size());
      s_->socket_->async_write_some(::asio::buffer(buf.begin(), buf.head_size()), send_handler<Session>(s_));
    } else {
      SPDLOG_TRACE(s_->log(), "send handler all bytes sent");
      s_->sending_ = false;
    }
  } else {
    s_->sending_ = false;
    s_->connected_ = false;
    s_->connecting_ = false;
    if (s_->reconnect_timer_) {
      s_->set_reconnect_timer(s_->reconnect_timer_timeout_, s_->reconnect_timer_timeout_);
    }
    switch (ec.value()) {
    case ::asio::error::operation_aborted:
      s_->on_operation_aborted();
      break;
    case ::asio::error::connection_reset:
      s_->on_connection_reset();
      break;
    case ::asio::error::eof:
      s_->on_eof();
      break;
    default:
      s_->log()->error("send failed on socket {0:#x}, error {}, {}", (uintptr_t)s_->socket_.get(), ec.value(), ec.message());
      s_->on_socket_error();
      break;
    }
  }
}

template <typename Session>
inline ares::network::receive_handler<Session>::receive_handler(std::shared_ptr<Session> s) :
  s_(s) {
}

template <typename Session>
inline ares::network::receive_handler<Session>::receive_handler(const receive_handler& other) :
  s_(other.s_) {
}

template <typename Session>
inline void ares::network::receive_handler<Session>::operator()(const std::error_code& ec, const size_t sz) {
  SPDLOG_TRACE(s_->log(), "receive handler begin, ec = {}, received_sz = {}", ec.value(), sz);
  if (ec.value() == 0) {
    SPDLOG_TRACE(s_->log(), "receive handler acquiring lock");
    std::unique_lock<std::mutex> lock(s_->recv_mutex_);
    if (sz > 0) {
      auto& buf = s_->recv_buf_;
      buf.adjust_size_by(sz);
      s_->reset_idle_timer();

      size_t need_more{0};
      while ((buf.size() > 0) && (need_more == 0)) {
        if (buf.size() >= 2) {
          // Need at least 2 bytes in buffer to dispatch by packet id
          if (buf.head_size() < 2) buf.defragment();
          auto packet_id = (uint16_t*)buf.begin();
          SPDLOG_TRACE(s_->log(), "receive handler calling dispatch, buf.size() = {}, raw packet id = {:#x}", buf.size(), *packet_id);
          need_more = s_->dispatch_packet(*packet_id);
          if (need_more == 0) s_->on_packet_processed();
          if (!s_->connected_) {
            s_->recv_buf_.clear();
          }
        } else {
          need_more = 2 - buf.size();
        }
      }
      lock.unlock();
      s_->receiving_ = false;
      if (need_more > 0) {
        // Receive the number of bytes needed reported by packet handler
        s_->receive(need_more);
      } else {
        // Wait till new packet id in buffer
        s_->receive(2);
      }
    } else {
      SPDLOG_TRACE(s_->log(), "received zero bytes, terminating session!");
      s_->server_.close_gracefuly(s_);
    }
  } else {
    s_->receiving_ = false;
    s_->connected_ = false;
    s_->connecting_ = false;
    if (s_->reconnect_timer_) {
      s_->set_reconnect_timer(s_->reconnect_timer_timeout_, s_->reconnect_timer_timeout_);
    }
    switch (ec.value()) {
    case ::asio::error::operation_aborted:
      s_->on_operation_aborted();
      break;
    case ::asio::error::connection_reset:
      s_->on_connection_reset();
      break;
    case ::asio::error::eof:
      s_->on_eof();
      break;
    default:
      s_->log()->error("receive failed on socket {0:#x}, error {}, {}", (uintptr_t)s_->socket_.get(), ec.value(), ec.message());
      s_->on_socket_error();
      break;
    }
  }
}

