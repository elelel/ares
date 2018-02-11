#include "state.hpp"
#include "../server.hpp"

void ares::character::zone_server::packet_handler<ares::packet::current<ares::packet::ATHENA_ZH_USER_COUNT>>::operator()() {
  SPDLOG_TRACE(log(), "ATHENA_ZH_USER_COUNT: begin");
  log()->warn("Received deprecated packet ATHENA_ZH_USER_COUNT, ignoring");
  SPDLOG_TRACE(log(), "ATHENA_ZH_USER_COUNT: end");
}
