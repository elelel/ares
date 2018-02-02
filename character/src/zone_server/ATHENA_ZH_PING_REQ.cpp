#include "state.hpp"
#include "../state.hpp"

void ares::character::zone_server::packet_handler<ares::packet_set, ares::packet::ATHENA_ZH_PING_REQ>::operator()() {
  SPDLOG_TRACE(server_state_.log(), "ATHENA_ZH_PING_REQ: begin");
  session_.emplace_and_send<packet_type<packet::ATHENA_HZ_PING_ACK>>();
  SPDLOG_TRACE(server_state_.log(), "ATHENA_ZH_PING_REQ: end");
}
