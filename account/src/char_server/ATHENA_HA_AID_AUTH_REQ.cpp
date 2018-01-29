#include "state.hpp"
#include "../state.hpp"

void ares::account::char_server::packet_handler<ares::packet::ATHENA_HA_AID_AUTH_REQ>::operator()() {
  SPDLOG_TRACE(log(), "handle_packet ATHENA_HA_AID_AUTH_REQ: begin");

  SPDLOG_TRACE(log(), "ATHENA_HA_AID_AUTH_REQ acquiring server lock");
  std::lock_guard<std::mutex> lock(server_state_.server.mutex());
  SPDLOG_TRACE(log(), "ATHENA_HA_AID_AUTH_REQ server lock acquired");

  auto found = server_state_.server.client_by_aid(p_->aid());
  if (found) {
    const auto& data = found->as_client();
    if ((data.auth_code1 == p_->auth_code1()) &&
        (data.auth_code2 == p_->auth_code2())) {
      session_.emplace_and_send<packet::ATHENA_AH_AID_AUTH_RESULT>(p_->aid(),
                                                                   p_->auth_code1(),
                                                                   p_->auth_code2(),
                                                                   p_->sex(),
                                                                   0,
                                                                   p_->request_id(),
                                                                   data.version,
                                                                   data.clienttype);
    } else {
      log()->info("AID {} authentication from char server failed, wrong auth codes", p_->aid());
      SPDLOG_TRACE(log(), "AID auth failure: data.auth_code1 = {}, p_->auth_code1 = {}, data.auth_code2 = {}, p_->auth_code2 = {}",
                   data.auth_code1,
                   p_->auth_code1(),
                   data.auth_code2,
                   p_->auth_code2());
      session_.emplace_and_send<packet::ATHENA_AH_AID_AUTH_RESULT>(p_->aid(),
                                                                   p_->auth_code1(),
                                                                   p_->auth_code2(),
                                                                   p_->sex(),
                                                                   1,
                                                                   p_->request_id(),
                                                                   data.version,
                                                                   data.clienttype);
    }
  } else {
    log()->warn("Char server requested AID authentication for non-authenticated AID {}", p_->aid());
    session_.emplace_and_send<packet::ATHENA_AH_AID_AUTH_RESULT>(p_->aid(),
                                                                 p_->auth_code1(),
                                                                 p_->auth_code2(),
                                                                 p_->sex(),
                                                                 1,
                                                                 p_->request_id(),
                                                                 0,
                                                                 0);
  }
  SPDLOG_TRACE(log(), "handle_packet ATHENA_HA_AID_AUTH_REQ: end");
}
