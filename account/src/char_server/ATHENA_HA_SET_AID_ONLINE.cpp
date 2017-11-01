#include "packet_handlers.hpp"

#include "state.hpp"
#include "../server.hpp"

void ares::account::char_server::packet_handler<ares::packet::ATHENA_HA_SET_AID_ONLINE>::operator()() {
  SPDLOG_TRACE(log(), "handle_packet ATHENA_HA_SET_AID_ONLINE: begin");
  SPDLOG_TRACE(log(), "ATHENA_HA_SET_AID_ONLINE acquiring server lock");
  std::lock_guard<std::mutex> lock(server_.mutex());
  SPDLOG_TRACE(log(), "ATHENA_HA_SET_AID_ONLINE server lock acquired");
  server_.link_aid_to_char_server(p_->aid(), session_.shared_from_this());
  SPDLOG_TRACE(log(), "handle_packet ATHENA_HA_SET_AID_ONLINE: end");
}
