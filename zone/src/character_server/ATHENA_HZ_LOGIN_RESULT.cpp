#include "state.hpp"
#include "../server.hpp"
#include "../character_server/state.hpp"

void ares::zone::character_server::packet_handler<ares::packet<ares::packets::ATHENA_HZ_LOGIN_RESULT>>::operator()() {
  SPDLOG_TRACE(log(), "handle_packet ATHENA_HZ_LOGIN_RESULT: begin");
  // TODO: Check if char server isn't closed
  if (p_->ErrorCode() == 0) {
    log()->info("Connected to character server");
    session_.emplace_and_send<packet<packets::ARES_ZH_MAP_NAMES_REQ>>(session_.recv_unfragmented_free_size() / 2);
  } else {
    log()->error("Failed to connect to character server, login result ErrorCode {}", p_->ErrorCode());
    throw network::terminate_session();
  }
  SPDLOG_TRACE(log(), "handle_packet ATHENA_HZ_LOGIN_RESULT: end");
}
