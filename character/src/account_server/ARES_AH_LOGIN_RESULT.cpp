#include "state.hpp"
#include "../server.hpp"

void ares::character::account_server::packet_handler<ares::packet::current<ares::packet::ARES_AH_LOGIN_RESULT>>::operator()() {
  SPDLOG_TRACE(log(), "ARES_AH_LOGIN_RESULT: begin");
  if (p_->error_code() == 0) {
    log()->info("Connected to account server");
    state_.reset_ping_account_server_timer();
  } else {
    log()->error("ARES_AH_LOGIN_RESULT account server rejected login request. Check login/password configuration");
    session_.close_gracefuly();
  }
  SPDLOG_TRACE(log(), "ARES_AH_LOGIN_RESULT: end");
}
