#pragma once

#include "../packet_handler.hpp"

template <typename Derived, typename Packet, typename Server, typename Session, typename SessionState>
inline ares::network::packet_handler<Derived, Packet, Server, Session, SessionState>::packet_handler(Server& serv, Session& sess, SessionState& session_state, std::shared_ptr<std::byte[]> buf) :
  p_(reinterpret_cast<Packet*>(buf.get())),
  session_(sess),
  server_(serv),
  state_(session_state),
  buf_(buf) {
}

template <typename Derived, typename Packet, typename Server, typename Session, typename SessionState>
inline ares::network::packet_handler<Derived, Packet, Server, Session, SessionState>::~packet_handler() {
}

template <typename Derived, typename Packet, typename Server, typename Session, typename SessionState>
inline auto ares::network::packet_handler<Derived, Packet, Server, Session, SessionState>::log() const -> std::shared_ptr<spdlog::logger> {
  return session_.log();
}

template <typename Derived, typename Packet, typename Server, typename Session, typename SessionState>
inline const elelel::network_buffer& ares::network::packet_handler<Derived, Packet, Server, Session, SessionState>::send_buf() const {
  return session_.send_buf_;
}
