#include "state.hpp"
#include "../server.hpp"

void ares::account::character_server::packet_handler<ares::packet::current<ares::packet::ARES_HA_USER_COUNT>>::operator()() {
  SPDLOG_TRACE(log(), "handle_packet ARES_HA_USER_COUNT begin");
  state_.user_count = p_->count();
  SPDLOG_TRACE(log(), "handle_packet ARES_HA_USER_COUNT end");
}
