#include "state.hpp"
#include "../server.hpp"

void ares::zone::character_server::packet_handler<ares::packet::current<ares::packet::ATHENA_HZ_PING_ACK>>::operator()() {
  SPDLOG_TRACE(log(), "handle_packet ATHENA_HZ_PING_ACK: begin");
  state_.reset_ping_character_server_timer();
  SPDLOG_TRACE(log(), "handle_packet ATHENA_HZ_PING_ACK: end");
}
