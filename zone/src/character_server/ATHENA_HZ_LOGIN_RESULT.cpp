#include "state.hpp"
#include "../server.hpp"
#include "../character_server/state.hpp"

void ares::zone::character_server::packet_handler<ares::packet::ATHENA_HZ_LOGIN_RESULT>::operator()() {
  SPDLOG_TRACE(log(), "handle_packet ATHENA_HZ_LOGIN_RESULT: begin");
  // TODO: Check if char server isn't closed
  if (p_->ErrorCode() == 0) {
    log()->info("Connected to character server");
  } else {
    log()->error("Failed to connect to character server, login result ErrorCode {}", p_->ErrorCode());
    throw network::terminate_session();
  }
  SPDLOG_TRACE(log(), "handle_packet ATHENA_HZ_LOGIN_RESULT: end");
}
