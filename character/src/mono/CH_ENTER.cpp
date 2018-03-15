#include "state.hpp"
#include "../server.hpp"
#include "../client/state.hpp"

void ares::character::mono::packet_handler<ares::packet::current<ares::packet::CH_ENTER>>::operator()() {
  SPDLOG_TRACE(log(), "CH_ENTER: begin");
  log()->info("CH_ENTER request AID = {}, AuthCode = {}, userLevel = {}, clientType = {}, Sex = {}",
              p_->AID().to_string(), p_->AuthCode(), p_->userLevel(), p_->clientType(), p_->Sex());
  if (p_->Sex() < 2) {
    SPDLOG_TRACE(log(), "Sending back AID {} in response to CH_ENTER", p_->AID().to_string());
    const auto aid = p_->AID();
    session_.copy_and_send(&aid, sizeof(aid));

    std::shared_ptr<session> found;
    {
      std::lock_guard<std::mutex> lock(server_.mutex());
      found = server_.find_client_session(model::account_id(p_->AID()));
    }
  
    if (found &&
        (p_->AuthCode() == found->as_client().auth_code1) &&
        ((int32_t)p_->userLevel() == found->as_client().auth_code2)) {
      // TODO: from zone server
    } else {
      // Login from account server
      SPDLOG_TRACE(log(), "Sending ATHENA_HA_AID_AUTH_REQ");
      uint32_t request_id{0};
      {
        std::lock_guard<std::mutex> lock(server_.mutex());
        auto s = session_.shared_from_this();
        request_id = server_.auth_requests->new_request(s);
      }
      state_.auth_code1 = p_->AuthCode();
      state_.auth_code2 = p_->userLevel();

      SPDLOG_TRACE(log(), "Account server existence check: {} {}", server_.account_server() != nullptr, request_id);
      server_.account_server()->emplace_and_send<packet::current<packet::ARES_HA_ACCOUNT_AUTH_REQ>>(request_id,
                                                                                                    model::account_id(p_->AID()),
                                                                                                    state_.auth_code1,
                                                                                                    state_.auth_code2
                                                                                                    );
    }
  } else {
    log()->warn("Unexpected sex {} in CH_ENTER packet from AID {}, closing session", p_->Sex(), p_->AID().to_string());
    session_.close_gracefuly();
  }
  SPDLOG_TRACE(log(), "CH_ENTER: end");
}

