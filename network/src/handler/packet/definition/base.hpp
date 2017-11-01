#pragma once

#include "../base.hpp"

#include "../../../packet_size.hpp"

template <typename Handler, typename Packet, typename Server, typename Session, typename State>
inline ares::network::handler::packet::base<Handler, Packet, Server, Session, State>::base(Server& server, Session& session, State& state) :
  server_(server),
  session_(session),
  state_(state),
  p_((Packet*)session_.recv_buf_.begin()),
  need_pop_(true) {
}

template <typename Handler, typename Packet, typename Server, typename Session, typename State>
inline ares::network::handler::packet::base<Handler, Packet, Server, Session, State>::~base() {
  SPDLOG_TRACE(session_.log_, "handler::packet::base::~base");
  if (need_pop_) {
    pop_packet();
  }
}

template <typename Handler, typename Packet, typename Server, typename Session, typename State>
inline void ares::network::handler::packet::base<Handler, Packet, Server, Session, State>::pop_packet() {
  SPDLOG_TRACE(session_.log_, "handler::packet::base::pop_packet need_pop_ = {}", need_pop_);
  if (need_pop_) {
    session_.recv_buf_.pop_front(packet_size());
    SPDLOG_TRACE(session_.log_, "after pop recv_buf_.size() = {}", session_.recv_buf_.size());
    need_pop_ = false;
  }
}

template <typename Handler, typename Packet, typename Server, typename Session, typename State>
inline size_t ares::network::handler::packet::base<Handler, Packet, Server, Session, State>::packet_size() const {
  return packet_size_helper::size<Packet>(p_, session_.recv_buf_.size());
}

template <typename Handler, typename Packet, typename Server, typename Session, typename State>
inline size_t ares::network::handler::packet::base<Handler, Packet, Server, Session, State>::handle() {
  auto& buf = session_.recv_buf_;
  // Since the Packet is defined, we have at least 2 bytes in buffer
  // Now check that we have the whole packet, both for static and dynamic packets
  auto needed = packet_size_helper::bytes_needed<Packet>(p_, buf.size());
  if (needed == 0) {
    if (buf.head_size() <= packet_size()) buf.defragment();
    SPDLOG_TRACE(session_.log_, "Invoking concrete packet handler, buf.size() = {}, packet_size = {}, packet {:#x}",
                 buf.size(),
                 packet_size(),
                 p_->PacketType);
    static_cast<Handler*>(this)->operator()();
    need_pop_ = true;
  } else {
    // Not whole packet received or parsed yet
    SPDLOG_TRACE(session_.log_, "need {} more bytes for the packet {:#x}",
                 packet_size_helper::bytes_needed<Packet>(p_, buf.size()),
                 p_->PacketType);
    need_pop_ = false;
  }
  return needed;
}

template <typename Handler, typename Packet, typename Server, typename Session, typename State>
inline std::shared_ptr<spdlog::logger>& ares::network::handler::packet::base<Handler, Packet, Server, Session, State>::log() {
  return session_.log_;
}
