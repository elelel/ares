#include "state.hpp"

#include "../server.hpp"

void ares::zone::mono::packet_handler<ares::packet::current<ares::packet::CZ_ENTER>>::operator()() {
  SPDLOG_TRACE(log(), "handle_packet CZ_ENTER: begin");
  SPDLOG_TRACE(log(), "CZ_ENTER: AID = {}, GID = {}, AuthCode = {}, clientTime = {}, Sex = {}",
               p_->AID().to_string(), p_->GID().to_string(), p_->AuthCode(), p_->clientTime(), p_->Sex());
  
  if (p_->AID().in_bounds<model::account_id>()) {
    if (p_->GID().in_bounds<model::character_id>()) {
      // TODO: Check for existing connections
      {
        state_.client_time_diff = model::to_host_epoch_time(std::chrono::system_clock::now()) - p_->clientTime();
        state_.auth_code1 = p_->AuthCode();
        state_.account_id = model::account_id(p_->AID());
        state_.character_id = model::character_id(p_->GID());
    
        SPDLOG_TRACE(log(), "Sending ARES_ZH_CHAR_AUTH_REQ AID = {}, GID = {}, account_id = {}, character_id = {}",
                     p_->AID().to_string(), p_->GID().to_string(), state_.account_id.to_string(), state_.character_id.to_string());
        uint32_t request_id;
        {
          std::lock_guard<std::mutex> lock(server_.auth_requests->mutex());
          request_id = server_.auth_requests->new_request(session_.shared_from_this());
        }
        server_.char_server()->emplace_and_send<packet::current<packet::ARES_ZH_CHAR_AUTH_REQ>>(request_id,
                                                                                                state_.account_id,
                                                                                                state_.character_id,
                                                                                                state_.auth_code1);
      }
    } else {
      log()->error("CZ_ENTER received GID {} which is not within character_id bounds, closing session", p_->GID().to_string());
      session_.emplace_and_send<packet::current<packet::ZC_REFUSE_ENTER>>(0);
      session_.close_gracefuly();
    }
  } else {
    log()->error("CZ_ENTER received AID {} which is not within account_id bounds, closing session", p_->AID().to_string());
    session_.emplace_and_send<packet::current<packet::ZC_REFUSE_ENTER>>(0);
    session_.close_gracefuly();
  }
  SPDLOG_TRACE(log(), "handle_packet CZ_ENTER: end");
}
