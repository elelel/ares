#include "state.hpp"
#include "../server.hpp"

void ares::account::character_server::packet_handler<ares::packet::current<ares::packet::ATHENA_HA_SET_AID_ONLINE>>::operator()() {
  SPDLOG_TRACE(log(), "handle_packet ATHENA_HA_SET_AID_ONLINE: begin");
  std::lock_guard<std::mutex> lock(server_.mutex());
  server_.link_aid_to_char_server(p_->aid(), session_.shared_from_this());
  SPDLOG_TRACE(log(), "handle_packet ATHENA_HA_SET_AID_ONLINE: end");
}
