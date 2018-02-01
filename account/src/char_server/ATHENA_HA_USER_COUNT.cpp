#include "state.hpp"
#include "../state.hpp"

void ares::account::char_server::packet_handler<ares::packet<ares::packets::ATHENA_HA_USER_COUNT>>::operator()() {
  SPDLOG_TRACE(log(), "handle_packet ATHENA_HA_USER_COUNT begin");
  session_state_.user_count = p_->count();
  SPDLOG_TRACE(log(), "handle_packet ATHENA_HA_USER_COUNT end");
}
