#include "state.hpp"
#include "../server.hpp"
#include "../character_server/state.hpp"

void ares::zone::character_server::packet_handler<ares::packet::current<ares::packet::ATHENA_HZ_LOGIN_RESULT>>::operator()() {
  SPDLOG_TRACE(log(), "handle_packet ATHENA_HZ_LOGIN_RESULT: begin");
  // TODO: Check if char server isn't closed
  if (p_->ErrorCode() == 0) {
    log()->info("Connected to character server, sending map names");
    session_.emplace_and_send<packet::current<packet::ARES_ZH_MAP_NAMES_REQ>>(32 * 1024);
  } else {
    log()->error("Failed to connect to character server, login result ErrorCode {}", p_->ErrorCode());
    server_.close_gracefuly(session_.shared_from_this());
  }
  SPDLOG_TRACE(log(), "handle_packet ATHENA_HZ_LOGIN_RESULT: end");
}
