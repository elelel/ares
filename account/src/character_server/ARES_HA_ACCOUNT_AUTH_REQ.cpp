#include "state.hpp"
#include "../server.hpp"

void ares::account::character_server::packet_handler<ares::packet::current<ares::packet::ARES_HA_ACCOUNT_AUTH_REQ>>::operator()() {
  SPDLOG_TRACE(log(), "handle_packet ARES_HA_ACCOUNT_AUTH_REQ: begin");
  std::shared_ptr<session> found;
  {
    std::lock_guard<std::mutex> lock(server_.mutex());
    found = server_.find_client_session(p_->account_id());
  }
  if (found) {
    const auto& data = found->as_client();
    if ((data.auth_code1 == p_->auth_code1()) &&
        (data.auth_code2 == p_->auth_code2())) {
      session_.emplace_and_send<packet::current<packet::ARES_AH_ACCOUNT_AUTH_RESULT>>(p_->request_id(),
                                                                                      p_->account_id(),
                                                                                      0);
    } else {
      log()->warn("Account id {} auth request from char server failed, wrong auth codes", p_->account_id().to_string());
      SPDLOG_TRACE(log(), "Account id auth failure: data.auth_code1 = {}, p_->auth_code1 = {}, data.auth_code2 = {}, p_->auth_code2 = {}",
                   data.auth_code1,
                   p_->auth_code1(),
                   data.auth_code2,
                   p_->auth_code2());
      session_.emplace_and_send<packet::current<packet::ARES_AH_ACCOUNT_AUTH_RESULT>>(p_->request_id(),
                                                                                      p_->account_id(),
                                                                                      1);
    }
  } else {
    log()->warn("Char server requested authentication for non-authenticated account id {}", p_->account_id().to_string());
    session_.emplace_and_send<packet::current<packet::ARES_AH_ACCOUNT_AUTH_RESULT>>(p_->request_id(),
                                                                                    p_->account_id(),
                                                                                    1);
  }
  
  SPDLOG_TRACE(log(), "handle_packet ARES_HA_ACCOUNT_AUTH_REQ: end");
}
