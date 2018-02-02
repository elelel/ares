#include "state.hpp"
#include "../state.hpp"

void ares::character::account_server::packet_handler<ares::packet_set, ares::packet::ATHENA_AH_LOGIN_RESULT>::operator()() {
  SPDLOG_TRACE(log(), "ATHENA_AH_LOGIN_RESULT: begin");
  if (p_->ErrorCode() == 0) {
    log()->info("Connected to login server");
    std::lock_guard<std::mutex> lock(server_state_.server.mutex());
    session_state_.ping_request_timer.fire();
    session_state_.send_aids_timer.fire();
    session_state_.send_user_count_timer.set();
  } else {
    log()->error("ATHENA_AH_LOGIN_RESULT account server rejected login request. Check login/password configuration");
    throw network::terminate_session();
  }
  SPDLOG_TRACE(log(), "ATHENA_AH_LOGIN_RESULT: end");
}
