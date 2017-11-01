#include "packet_handlers.hpp"
 
#include "state.hpp"
#include "../server.hpp"

void ares::account::char_server::packet_handler<ares::packet::ATHENA_HA_USER_COUNT>::operator()() {
  SPDLOG_TRACE(log(), "handle_packet ATHENA_HA_USER_COUNT begin");
  state_.user_count = p_->count();
  SPDLOG_TRACE(log(), "handle_packet ATHENA_HA_USER_COUNT end");
}
