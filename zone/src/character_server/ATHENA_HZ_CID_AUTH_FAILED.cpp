#include "state.hpp"
#include "../server.hpp"

void ares::zone::character_server::packet_handler<ares::packet::current<ares::packet::ATHENA_HZ_CID_AUTH_FAILED>>::operator()() {
  SPDLOG_TRACE(log(), "handle_packet ATHENA_HZ_CID_AUTH_FAILED: begin");
  log()->warn("Authentication failed for aid {}, cid {}", p_->aid(), p_->cid());
  SPDLOG_TRACE(log(), "handle_packet ATHENA_HZ_CID_AUTH_FAILED: end");
}
