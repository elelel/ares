#pragma once

#include "../packet_handler.hpp"

template <typename Derived, typename Packet, typename Server, typename Session, typename SessionState>
inline ares::network::packet_handler<Derived, Packet, Server, Session, SessionState>::packet_handler(Server& serv, Session& sess, SessionState& session_state, std::shared_ptr<Packet> p) :
  p_(p),
  session_(sess),
  server_(serv),
  state_(session_state) {
}

template <typename Derived, typename Packet, typename Server, typename Session, typename SessionState>
inline ares::network::packet_handler<Derived, Packet, Server, Session, SessionState>::~packet_handler() {
}

/*
template <typename Derived, typename Packet, typename Server, typename Session, typename SessionState>
inline void ares::network::packet_handler<Derived, Packet, Server, Session, SessionState>::pop_packet() {
  if (need_pop_) {
    session_.recv_buf_.pop_front(packet_size());
    need_pop_ = false;
  }
}

template <typename Derived, typename Packet, typename Server, typename Session, typename SessionState>
inline size_t ares::network::packet_handler<Derived, Packet, Server, Session, SessionState>::packet_size() const {
  return packet::size::get<handled_packet_type>(p_, session_.recv_buf_.size());
}
*/

template <typename Derived, typename Packet, typename Server, typename Session, typename SessionState>
inline auto ares::network::packet_handler<Derived, Packet, Server, Session, SessionState>::log() const -> std::shared_ptr<spdlog::logger> {
  return session_.log();
}

template <typename Derived, typename Packet, typename Server, typename Session, typename SessionState>
inline const elelel::network_buffer& ares::network::packet_handler<Derived, Packet, Server, Session, SessionState>::send_buf() const {
  return session_.send_buf_;
}
