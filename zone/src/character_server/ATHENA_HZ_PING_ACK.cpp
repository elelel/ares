#include "state.hpp"
#include "../server.hpp"

void ares::zone::character_server::packet_handler<ares::packet<ares::packets::ATHENA_HZ_PING_ACK>>::operator()() {
  SPDLOG_TRACE(log(), "handle_packet ATHENA_HZ_PING_ACK: begin");
  session_.as_character_server().ping_timeout_timer.cancel();
  session_.as_character_server().ping_request_timer.set();
  SPDLOG_TRACE(log(), "handle_packet ATHENA_HZ_PING_ACK: end");
}

