#include "state.hpp"
#include "../server.hpp"

void ares::zone::character_server::packet_handler<ares::packet::current<ares::packet::ARES_HZ_CID_AUTH_OK>>::operator()() {
  SPDLOG_TRACE(log(), "handle_packet ATHENA_HZ_CID_AUTH_OK: begin");
  log()->warn("Authentication ok for aid {}", p_->aid());
  SPDLOG_TRACE(log(), "handle_packet ATHENA_HZ_CID_AUTH_OK: end");
}
