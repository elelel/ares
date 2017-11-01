#include "packet_handlers.hpp"

#include "state.hpp"
#include "../server.hpp"

void ares::character::account_server::packet_handler<ares::packet::ATHENA_AH_PING_ACK>::operator()() {
  SPDLOG_TRACE(log(), "handle_packet ATHENA_AH_PING_ACK: begin");
  session_.as_account_server().ping_timeout_timer.cancel();
  session_.as_account_server().ping_request_timer.set();
  SPDLOG_TRACE(log(), "handle_packet ATHENA_AH_PING_ACK: end");
}

