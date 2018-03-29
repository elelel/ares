#pragma once

#include "../handlers.hpp"

#include <mutex>

template <typename Session>
inline ares::network::handler::session_base<Session>::session_base(std::shared_ptr<Session> s) :
  session_(s) {
  if (session_ == nullptr) throw std::runtime_error("Attempt to construct session handler with session == nullptr");
};

template <typename Session>
inline ares::network::handler::session_base<Session>::session_base(const session_base<Session>& other) :
  session_(other.session_) {
  if (session_ == nullptr) throw std::runtime_error("Attempt to copy-construct session handler with session == nullptr");
}

template <typename Session>
inline ares::network::handler::session_base<Session>::session_base(session_base<Session>&& other) :
  session_(std::move(other.session_)) {
  if (session_ == nullptr) throw std::runtime_error("Attempt to move-construct session handler with session == nullptr");
}

template <typename Session>
inline void ares::network::handler::session_base<Session>::act_on_error(const std::error_code& ec, const std::string& handler_name) {
  auto& s = *session_;
  s.connected_ = false;
  s.connecting_ = false;
  if (s.reconnect_timer_) {
    s.set_reconnect_timer(s.reconnect_timer_timeout_, s.reconnect_timer_timeout_);
  }
  switch (ec.value()) {
  case ::asio::error::operation_aborted:
    SPDLOG_TRACE(s.log(), "session_base::act_on_error operation_aborted");
    s.on_operation_aborted();
    break;
  case ::asio::error::connection_reset:
    SPDLOG_TRACE(s.log(), "session_base::act_on_error connection_reset");
    s.on_connection_reset();
    break;
  case ::asio::error::eof:
    SPDLOG_TRACE(s.log(), "session_base::act_on_error on_eof");
    s.on_eof();
    break;
  default:
    s.log()->error("session_base::act_on_error '{}' failed on socket {}, error {}, {}",
                   handler_name, (void*)s.socket_.get(), ec.value(), ec.message());
    s.on_socket_error();
    break;
  }
}

template <typename Session>
inline ares::network::handler::reconnect_timer<Session>::reconnect_timer(std::shared_ptr<Session> s,
                                                                         const std::chrono::seconds timeout) :
  session_base<Session>(s),
  timeout_(timeout) {
  };

template <typename Session>
inline ares::network::handler::reconnect_timer<Session>::reconnect_timer(const reconnect_timer<Session>& other) :
  session_base<Session>(other),
  timeout_(other.timeout_) {
}

template <typename Session>
inline ares::network::handler::reconnect_timer<Session>::reconnect_timer(reconnect_timer<Session>&& other) :
  session_base<Session>(std::move(other)),
  timeout_(other.timeout_) {
}

template <typename Session>
inline void ares::network::handler::reconnect_timer<Session>::operator()(const std::error_code& ec) {
  auto& s = *this->session_;
  std::stringstream ss;
  ss << *s.connect_ep_;
  if (ec.value() == 0) {
    if (!s.connecting_ && !s.connected_) {
      s.connecting_ = true;
      s.log()->info("Establishing connection with {}", ss.str());
      SPDLOG_TRACE(s.log(), "Locking send mutex to clear buffer");
      {
        std::lock_guard<std::mutex> lock(s.send_mutex_);
        s.send_buf_.clear();
      }
      s.socket_ = std::make_shared<asio::ip::tcp::socket>(*s.io_context());
      s.idle_timer_->cancel();
      std::error_code ec;
      s.socket_->open(s.connect_ep_->protocol(), ec);
      if (ec.value() == 0) {
        s.socket_->async_connect(*s.connect_ep_, [s = this->session_] (const std::error_code& ec) {
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
        s.log()->warn("Connection to {} failed while opening the socket with error {}, {}", ss.str(), ec.value(), ec.message());
        s.set_reconnect_timer(s.reconnect_timer_timeout_, s.reconnect_timer_timeout_);
      }
    } else {
      s.log()->warn("Connection to {} is still established or in process of being established, ignoring reconnection request", ss.str());
    }
  } else {
    SPDLOG_TRACE(s.log(), "Reconnect timer error {}, {}", ec.value(), ec.message());
  }
}

template <typename Session>
inline ares::network::handler::close_gracefuly_timer<Session>::close_gracefuly_timer(std::shared_ptr<Session> s) :
  session_base<Session>(s) {
}

template <typename Session>
inline ares::network::handler::close_gracefuly_timer<Session>::close_gracefuly_timer(const close_gracefuly_timer<Session>& other) :
  session_base<Session>(other) {
}

template <typename Session>
inline ares::network::handler::close_gracefuly_timer<Session>::close_gracefuly_timer(close_gracefuly_timer<Session>&& other) :
  session_base<Session>(std::move(other)) {
}

template <typename Session>
inline void ares::network::handler::close_gracefuly_timer<Session>::operator()(const std::error_code& ec) {
  auto& s = *this->session_;
  if (ec.value() == 0) {
    s.close_abruptly();
  } else {
    SPDLOG_TRACE(s.log(), "Close gracefuly timer error {}, {}", ec.value(), ec.message());
  }
}

template <typename Session>
inline ares::network::handler::send<Session>::send(std::shared_ptr<Session> s) :
  session_base<Session>(s) {
}

template <typename Session>
inline ares::network::handler::send<Session>::send(const send& other) :
  session_base<Session>(other) {
}

template <typename Session>
inline ares::network::handler::send<Session>::send(send&& other) :
  session_base<Session>(std::move(other)) {
}

template <typename Session>
inline void ares::network::handler::send<Session>::operator()(const std::error_code& ec, const size_t sent_sz) {
  auto& s = *this->session_;
  if (ec.value() == 0) {
    SPDLOG_TRACE(s.log(), "send handler ackquiring lock");
    std::lock_guard<std::mutex> lock(s.send_mutex_);
    auto& buf = s.send_buf_;
    buf.pop_front(sent_sz);
    SPDLOG_TRACE(s.log(), "send handler removed {} bytes from send buf", sent_sz);
    if (buf.size() > 0) {
      SPDLOG_TRACE(s.log(), "send handler still {} bytes left to send", buf.size());
      s.socket_->async_write_some(::asio::buffer(buf.begin(), buf.head_size()), send<Session>(this->session_));
    } else {
      SPDLOG_TRACE(s.log(), "send handler all bytes sent");
      s.sending_ = false;
    }
  } else {
    s.sending_ = false;
    this->act_on_error(ec, "send");
  }
}

template <typename Session>
inline ares::network::handler::receive_id<Session>::receive_id(std::shared_ptr<Session> s) :
  session_base<Session>(s),
  need_more_(sizeof(s->packet_id_recv_buf_)) {
}

template <typename Session>
inline ares::network::handler::receive_id<Session>::receive_id(const receive_id& other) :
  session_base<Session>(other),
  need_more_(other.need_more_) {
}

template <typename Session>
inline ares::network::handler::receive_id<Session>::receive_id(receive_id&& other) :
  session_base<Session>(std::move(other)),
  need_more_(other.need_more_) {
}

template <typename Session>
inline void ares::network::handler::receive_id<Session>::operator()(const std::error_code& ec, const size_t sz) {
  auto& s = *this->session_;
  SPDLOG_TRACE(s.log(), "receive id handler begin, ec = {}, received_sz = {}", ec.value(), sz);
  if (ec.value() == 0) {
    s.reset_idle_timer();
    if (sz > 0) {
      std::unique_lock<std::mutex> lock(s.recv_mutex_);
      auto& packet_id = s.packet_id_recv_buf_;
      if (need_more_ > 0) {
        // receiving packet_id
        need_more_ -= sz;
        if (need_more_ == 0) {
          // Deobfuscate packet_id if needed and
          // get buffer size, expected packet size, PacketLength offset for this packet id
          auto alloc_sizes = s.packet_sizes(packet_id);
          SPDLOG_TRACE(s.log(), "For packed id {:#x} got buffer size {}, expected size {}, PacketLength offset {}",
                       packet_id, std::get<0>(alloc_sizes), std::get<1>(alloc_sizes), std::get<2>(alloc_sizes));
          if (std::get<0>(alloc_sizes) != 0) {
            try {
              auto ai = memory::packet_alloc(alloc_sizes);
              // Place packet_id into memory chunk's start
              auto pck_id = (decltype(s.packet_id_recv_buf_)*)ai.buf.get();
              *pck_id = packet_id;
              // Receive the rest of the packet if the packet is expected to have something more than just id
              if (ai.expected_sz > sizeof(packet_id)) {
                auto buf = asio::buffer((void*)((uintptr_t)ai.buf.get() + sizeof(packet_id)), ai.expected_sz - sizeof(packet_id));
                s.socket_->async_read_some(buf, handler::receive_after_id<Session>(this->session_, std::move(ai), sizeof(packet_id)));
              } else {
                s.dispatch_packet(ai.buf);
                s.receiving_ = false;
                s.receive();
              }
            } catch (std::bad_alloc) {
              s.log()->error("Failed to allocate memory for packet {:#x}, closing session", packet_id);
              s.close_gracefuly();
            }
          } else {
            s.log()->error("Packet id {:#x} is not known to this server under selected packet set, closing session", packet_id);
            s.close_gracefuly();
          }
        } else if (need_more_ < sizeof(packet_id)) {
          // Not whole packet_id received, request more bytes
          auto h{*this};
          s.socket_->async_read_some(asio::buffer((void*)((uintptr_t)&packet_id + (sizeof(packet_id) - need_more_)), need_more_),
                                     std::move(h));
        } else {
          s.log()->error("receive received more bytes than needed (receiving packet_id)");
          s.close_gracefuly();
        }
      } else {
        s.log()->error("receive received triggered when no more bytes needed (receiving packet_id)");
        s.close_gracefuly();
      }
    } else {
      SPDLOG_TRACE(s.log(), "received zero bytes, terminating session (receiving packet_id)!");
      s.close_gracefuly();
    }
  } else {
    s.receiving_ = false;
    this->act_on_error(ec, "receive packet id");
  }
}

template <typename Session>
inline ares::network::handler::receive_after_id<Session>::receive_after_id(std::shared_ptr<Session> s,
                                                                           memory::packet_alloc&& ai,
                                                                           const size_t already_received) :
  session_base<Session>(s),
  ai_(std::move(ai)),
  bytes_received_(already_received) {
  }

template <typename Session>
ares::network::handler::receive_after_id<Session>::receive_after_id(const receive_after_id& other) :
  session_base<Session>(other),
  ai_(other.ai_),
  bytes_received_(bytes_received_) {
  }

template <typename Session>
ares::network::handler::receive_after_id<Session>::receive_after_id(receive_after_id&& other) :
  session_base<Session>(std::move(other)),
  ai_(std::move(other.ai_)),
  bytes_received_(std::move(other.bytes_received_)) {
  }

template <typename Session>
inline void ares::network::handler::receive_after_id<Session>::operator()(const std::error_code& ec, const size_t sz) {
  auto& s = *this->session_;
  SPDLOG_TRACE(s.log(), "receive after id handler begin, ec = {}, received_sz = {}", ec.value(), sz);
  if (ec.value() == 0) {
    s.reset_idle_timer();
    if (sz > 0) {
      std::unique_lock<std::mutex> lock(s.recv_mutex_);
      bytes_received_ += sz;
      // Check if we need more bytes. We may need more for two reasons: we still haven't got what we've already requested
      // or we already have PacketLength field's contents and it indicates that we must change our expected_packet_sz
      int need_more = int(ai_.expected_sz) - int(bytes_received_);
      int need_more_dyn_length = 0;
      if ((ai_.PacketLength_offset != 0) && (bytes_received_ >= ai_.PacketLength_offset + sizeof(uint16_t))) {
        auto PacketLength = (uint16_t*)((uintptr_t)ai_.buf.get() + ai_.PacketLength_offset);
        ai_.expected_sz = *PacketLength;
        need_more_dyn_length = int(*PacketLength) - int(bytes_received_);
      }
      need_more = need_more > need_more_dyn_length ? need_more : need_more_dyn_length;
      if (need_more == 0) {
        s.dispatch_packet(ai_.buf);
        s.receiving_ = false;
        s.receive();
      } else if (need_more > 0) {
        // We need to receieve more data, but first we should check that our allocation fits that
        if (ai_.expected_sz <= ai_.buf_sz) {
          SPDLOG_TRACE(s.log(), "receive_after_id need to receive more bytes with existing allocation");
          auto buf_ptr = (void*)((uintptr_t)ai_.buf.get() + bytes_received_);
          s.socket_->async_read_some(asio::buffer(buf_ptr, need_more),
                                     handler::receive_after_id<Session>(this->session_, std::move(ai_), bytes_received_));
        } else {
          SPDLOG_TRACE(s.log(), "receive_after_id need to receive more bytes with new traditional allocation");
          // We are out of our clever preallocation scheme, fallback to direct memory allocation
          auto old_buf = ai_.buf;
          try {
            ai_ = memory::packet_alloc(ai_.expected_sz, ai_.expected_sz, ai_.PacketLength_offset);
            memcpy(ai_.buf.get(), old_buf.get(), bytes_received_);
            auto buf_ptr = (void*)((uintptr_t)ai_.buf.get() + bytes_received_);
            s.socket_->async_read_some(asio::buffer(buf_ptr, need_more),
                                       handler::receive_after_id<Session>(this->session_, std::move(ai_), bytes_received_));
          } catch (std::bad_alloc) {
            s.log()->error("failed to allocate new buffer in malloc fallback scheme, closing session");
            s.close_gracefuly();
          }
        }
      } else {
        s.log()->error("received more bytes than expected, closing session");
        s.close_gracefuly();
      }
    } else {
      SPDLOG_TRACE(s.log(), "received zero bytes, terminating session (receiving packet_id)!");
      s.close_gracefuly();
    }
  } else {
    s.receiving_ = false;
    this->act_on_error(ec, "receive packet id");
  }
}
