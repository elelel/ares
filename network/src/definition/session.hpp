#pragma once

#include "../session.hpp"
#include "../exceptions.hpp"

template <typename Session>
ares::network::session<Session>::session(std::shared_ptr<boost::asio::io_service> io_service,
                                         std::shared_ptr<spdlog::logger> log,
                                         std::shared_ptr<boost::asio::ip::tcp::socket> socket) :
  io_service_(io_service),
  log_(log), 
  socket_(socket),
  connected_(true),
  inactivity_timer_(boost::asio::steady_timer{*io_service_}),
  receiving_(false),
  sending_(false) {
  if (socket_) {
    // TODO: Make configurable
    socket_->set_option(boost::asio::ip::tcp::no_delay(true));
  } else {
    connected_ = false;
  }
  }

template <typename Session>
inline void ares::network::session<Session>::receive(const size_t receive_sz) {
  if (!receiving_) {
    receiving_ = true;
    std::lock_guard<std::mutex> lock(recv_mutex_); // Not needed?
    if (!socket_) return;
    auto sz = receive_sz;
    if (receive_sz == 0) {
      sz = recv_buf_.unfragmented_free_size();
    }
    if (recv_buf_.free_size() >= sz) {
      auto handler = static_cast<Session&>(*this).make_recv_handler();
      if (recv_buf_.unfragmented_free_size() < sz) recv_buf_.defragment();
      SPDLOG_TRACE(log_, "receive requesting {} bytes", sz);
      this->socket_->async_read_some(boost::asio::buffer(recv_buf_.end(), sz), handler);
    } else {
      log_->error("receive: out of buffer space, session {0:#x}", (uintptr_t)this);
      throw out_of_buffer_space();
    }
  }
}

template <typename Session>
inline ares::network::session<Session>::~session() {
  SPDLOG_TRACE(log_, "ares::network::session<Session>::~session");
}

template <typename Session>
template <typename Packet>
inline void ares::network::session<Session>::copy_and_send(const Packet& p) {
  SPDLOG_TRACE(log_, "network::session::send(Packet) acquiring send lock");
  std::unique_lock<std::mutex> lock(send_mutex_);
  if (!socket_) return;
  SPDLOG_TRACE(log_, "network::session::send(Packet) send lock acquired");  
  if (packet_size_helper::validate<Packet>(p)) {
    const auto send_sz = packet_size_helper::size<Packet>(&p, sizeof(Packet));
    SPDLOG_TRACE(log_, "network::session::send(Packet) queueing {} bytes to send", send_sz); 
    if (send_sz > 0) {
      lock.unlock();
      copy_and_send(&p, send_sz);
    } else {
      log_->error("Packet send failed, can't determine size to send");
      throw packet_size_error();
    }
  } else {
    log_->error("Packet send failed, size validation failed, "
                 "sizeof(Packet) = {}",
                 sizeof(Packet));
    throw packet_size_error();
  }
}

template <typename Session>
inline void ares::network::session<Session>::send_wake_up(const void* data_start) {
  if (!sending_) {
    sending_ = true;
    SPDLOG_TRACE(log_, "Send handler wakeup, {} bytes to send", send_buf_.size());
    auto handler = static_cast<Session&>(*this).make_send_handler();
    socket_->async_write_some(boost::asio::buffer(data_start, send_buf_.head_size()), handler);
  }
}

template <typename Session>
inline void ares::network::session<Session>::copy_and_send(const void* data, const size_t send_sz) {
  SPDLOG_TRACE(log_, "network::session::send acquiring send lock");
  std::lock_guard<std::mutex> lock(send_mutex_);
  if (!socket_) return;
  SPDLOG_TRACE(log_, "network::session::send send lock acquired");
  if (send_buf_.free_size() >= send_sz) {
    const auto p = send_buf_.end();
    SPDLOG_TRACE(log_, "sending {} bytes, copying to {:#x}, offset from buf start {}, already in buf {}",
                 send_sz, (uintptr_t)send_buf_.end(), (uintptr_t)send_buf_.end() - (uintptr_t)send_buf_.begin(), send_buf_.size());
    send_buf_.push_back(data, send_sz);
    send_wake_up(p);
  } else {
    // TODO: autogrow, but make sure asio is not using the buffer
    log_->error("send: out of buffer space, session {0:#x}", (uintptr_t)this);
    throw out_of_buffer_space();
  }
}

template <typename Session>
template <typename Packet, typename... Args>
inline void ares::network::session<Session>::emplace_and_send(Args&&... args) {
  SPDLOG_TRACE(log_, "emplace_and_send acquiring send lock");
  std::lock_guard<std::mutex> lock(send_mutex_);
  if (!socket_) return;
  SPDLOG_TRACE(log_, "emplace_and_send lock acquired");
  const auto header_sz = sizeof(Packet);
  if (header_sz <= send_buf_.free_size()) {
    if (header_sz > send_buf_.unfragmented_free_size()) {
      if (sending_) {
        // Fallback to copy-send, because we can't defragment buffer space while asio is busy
        Packet pck(std::forward<Args>(args)...);
        copy_and_send<Packet>(pck);
        return;
      } else {
        send_buf_.defragment();
      }
    }
    auto p = (Packet*)send_buf_.end();
    p->emplace(std::forward<Args>(args)...);
    send_buf_.adjust_size_by(header_sz);
    send_wake_up(p);
  } else {
    // TODO: autogrow, but make sure asio is not using the buffer
    log_->error("send: out of buffer space, session {0:#x}", (uintptr_t)this);
    throw out_of_buffer_space();
  }
}

template <typename Session>
inline void ares::network::session<Session>::close_socket() {
  if (socket_) {
    SPDLOG_TRACE(log_, "closing socket");
    try {
      socket_->shutdown(boost::asio::ip::tcp::socket::shutdown_both); 
    } catch (...) {
    }
    socket_->cancel();
    socket_->close();
    socket_ = nullptr;
    receiving_ = false;
    sending_ = false;
  }
}

template <typename Session>
inline std::shared_ptr<boost::asio::io_service> ares::network::session<Session>::io_service() const {
  return io_service_;
}

template <typename Session>
inline std::shared_ptr<boost::asio::ip::tcp::socket>& ares::network::session<Session>::socket() {
  return socket_;
}

template <typename Session>
inline bool ares::network::session<Session>::connected() const {
  return (socket_ != nullptr) && (connected_);
}

template <typename Session>
inline void ares::network::session<Session>::set_connected() {
  connected_ = true;
}

template <typename Session>
inline std::atomic<bool>& ares::network::session<Session>::sending() {
  return sending_;
}
