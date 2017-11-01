#include "packet_handlers.hpp"

#include "state.hpp"
#include "../server.hpp"

void ares::account::char_server::packet_handler<ares::packet::ATHENA_HA_SET_AID_OFFLINE>::operator()() {
  SPDLOG_TRACE(log(), "handle_packet ATHENA_HA_SET_AID_OFFLINE: begin");
  SPDLOG_TRACE(log(), "ATHENA_HA_SET_AID_OFFLINE acquiring server lock");
  std::lock_guard<std::mutex> lock(server_.mutex());
  SPDLOG_TRACE(log(), "ATHENA_HA_SET_AID_OFFLINE server lock acquired");
  server_.unlink_aid_from_char_server(p_->aid(), session_.shared_from_this());  
  SPDLOG_TRACE(log(), "handle_packet ATHENA_HA_SET_AID_OFFLINE: end");
}
