#include "state.hpp"
#include "../server.hpp"
#include "../character_server/state.hpp"

void ares::zone::character_server::packet_handler<ares::packet::current<ares::packet::ARES_HZ_LOGIN_RESULT>>::operator()() {
  SPDLOG_TRACE(log(), "handle_packet ARES_HZ_LOGIN_RESULT: begin");
  // TODO: Check if char server isn't closed
  if (p_->error_code() == 0) {
    log()->info("Connected to character server, sending map names request");
    session_.emplace_and_send<packet::current<packet::ARES_ZH_MAP_IDS_REQ>>(32 * 1024);
    state_.reset_ping_character_server_timer();
    // TODO: start timer with ARES_ZH_PING_REQ
  } else {
    log()->error("Failed to connect to character server, login result error_code {}", p_->error_code());
    session_.close_gracefuly();
  }
  SPDLOG_TRACE(log(), "handle_packet ARES_HZ_LOGIN_RESULT: end");
}
