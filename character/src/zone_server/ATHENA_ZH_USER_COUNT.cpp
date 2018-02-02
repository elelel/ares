#include "state.hpp"
#include "../state.hpp"

void ares::character::zone_server::packet_handler<ares::packet_set, ares::packet::ATHENA_ZH_USER_COUNT>::operator()() {
  SPDLOG_TRACE(server_state_.log(), "ATHENA_ZH_USER_COUNT: begin");
  log()->warn("Received deprecated packet ATHENA_HA_USER_COUNT, ignoring");
  SPDLOG_TRACE(server_state_.log(), "ATHENA_ZH_USER_COUNT: end");
}
