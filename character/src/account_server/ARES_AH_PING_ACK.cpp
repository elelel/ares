#include "state.hpp"
#include "../server.hpp"

void ares::character::account_server::packet_handler<ares::packet::current<ares::packet::ARES_AH_PING_ACK>>::operator()() {
  SPDLOG_TRACE(log(), "handle_packet ARES_AH_PING_ACK: begin");
  state_.reset_ping_account_server_timer();
  SPDLOG_TRACE(log(), "handle_packet ARES_AH_PING_ACK: end");
}

