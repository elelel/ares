#pragma once

#include "../packet_handler.hpp"

template <typename Derived, typename PacketSet, typename PacketName, typename Server, typename Session, typename SessionState>
inline ares::network::packet_handler<Derived, PacketSet, PacketName, Server, Session, SessionState>::packet_handler(Server& serv, Session& sess, SessionState& session_state) :
  p_((handled_packet_type*)sess.recv_buf_.begin()),
  session_(sess),
  server_(serv),
  state_(session_state),
  need_pop_(true) {
}

template <typename Derived, typename PacketSet, typename PacketName, typename Server, typename Session, typename SessionState>
inline ares::network::packet_handler<Derived, PacketSet, PacketName, Server, Session, SessionState>::~packet_handler() {
  if (need_pop_) pop_packet();
}

template <typename Derived, typename PacketSet, typename PacketName, typename Server, typename Session, typename SessionState>
inline void ares::network::packet_handler<Derived, PacketSet, PacketName, Server, Session, SessionState>::pop_packet() {
  if (need_pop_) {
    session_.recv_buf_.pop_front(packet_size());
    need_pop_ = false;
  }
}

template <typename Derived, typename PacketSet, typename PacketName, typename Server, typename Session, typename SessionState>
inline size_t ares::network::packet_handler<Derived, PacketSet, PacketName, Server, Session, SessionState>::packet_size() const {
  return packet::size::get<handled_packet_type>(p_, session_.recv_buf_.size());
}

template <typename Derived, typename PacketSet, typename PacketName, typename Server, typename Session, typename SessionState>
inline auto ares::network::packet_handler<Derived, PacketSet, PacketName, Server, Session, SessionState>::log() const -> std::shared_ptr<spdlog::logger> {
  return session_.log();
}

template <typename Derived, typename PacketSet, typename PacketName, typename Server, typename Session, typename SessionState>
inline const elelel::network_buffer& ares::network::packet_handler<Derived, PacketSet, PacketName, Server, Session, SessionState>::recv_buf() const {
  return session_.recv_buf_;
}

template <typename Derived, typename PacketSet, typename PacketName, typename Server, typename Session, typename SessionState>
inline const elelel::network_buffer& ares::network::packet_handler<Derived, PacketSet, PacketName, Server, Session, SessionState>::send_buf() const {
  return session_.send_buf_;
}

template <typename Derived, typename PacketSet, typename PacketName, typename Server, typename Session, typename SessionState>
inline size_t ares::network::packet_handler<Derived, PacketSet, PacketName, Server, Session, SessionState>::handle() {
  auto& buf = session_.recv_buf_;
  // Since the Packet is defined, we have at least 2 bytes in buffer
  // Now check that we have the whole packet, both for static and dynamic packets
  auto more_needed = packet::size::bytes_needed<handled_packet_type>(p_, buf.size());
  if (more_needed == 0) {
    if (buf.head_size() <= packet_size()) {
      // We are called with recv_mutex_ locked, safe to defragment
      buf.defragment();
    }
    SPDLOG_TRACE(session_.log(), "Invoking concrete packet handler for packet_id {:#x}, buf.size() = {}, packet_size = {}",
                 p_->PacketType,
                 buf.size(),
                 packet_size());
    static_cast<Derived*>(this)->operator()();
    need_pop_ = true;
  } else {
    // Not whole packet received or parsed yet
    SPDLOG_TRACE(session_.log(), "Need {} more bytes for the packet {:#x}", more_needed, p_->PacketType);
    need_pop_ = false;
  }
  return more_needed;
}
