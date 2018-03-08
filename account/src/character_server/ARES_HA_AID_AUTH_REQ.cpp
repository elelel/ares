#include "state.hpp"
#include "../server.hpp"

void ares::account::character_server::packet_handler<ares::packet::current<ares::packet::ARES_HA_AID_AUTH_REQ>>::operator()() {
  SPDLOG_TRACE(log(), "handle_packet ARES_HA_AID_AUTH_REQ: begin");
  std::shared_ptr<session> found;
  {
    std::lock_guard<std::mutex> lock(server_.mutex());
    found = server_.client_by_aid(p_->aid());
  }
  if (found) {
    const auto& data = found->as_client();
    if ((data.auth_code1 == p_->auth_code1()) &&
        (data.auth_code2 == p_->auth_code2())) {
      session_.emplace_and_send<packet::current<packet::ARES_AH_AID_AUTH_RESULT>>(p_->request_id(),
                                                                                  p_->aid(),
                                                                                  0);
    } else {
      log()->warn("AID {} authentication from char server failed, wrong auth codes", p_->aid());
      SPDLOG_TRACE(log(), "AID auth failure: data.auth_code1 = {}, p_->auth_code1 = {}, data.auth_code2 = {}, p_->auth_code2 = {}",
                   data.auth_code1,
                   p_->auth_code1(),
                   data.auth_code2,
                   p_->auth_code2());
      session_.emplace_and_send<packet::current<packet::ARES_AH_AID_AUTH_RESULT>>(p_->request_id(),
                                                                                  p_->aid(),
                                                                                  1);
    }
  } else {
    log()->warn("Char server requested AID authentication for non-authenticated AID {}", p_->aid());
    session_.emplace_and_send<packet::current<packet::ARES_AH_AID_AUTH_RESULT>>(p_->request_id(),
                                                                                p_->aid(),
                                                                                1);
  }
  
  SPDLOG_TRACE(log(), "handle_packet ARES_HA_AID_AUTH_REQ: end");
}
