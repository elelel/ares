#include "state.hpp"

#include "../server.hpp"

void ares::zone::mono::packet_handler<ares::packet::current<ares::packet::CZ_ENTER>>::operator()() {
  SPDLOG_TRACE(log(), "handle_packet CZ_ENTER: begin");

  SPDLOG_TRACE(log(), "CZ_ENTER: AID = {}, GID = {}, AuthCode = {}, clientTime = {}, Sex = {}",
               p_->AID(), p_->GID(), p_->AuthCode(), p_->clientTime(), p_->Sex());

  // TODO: Check for existing connections
  
  {
    state_.client_time_diff = model::to_host_epoch_time(std::chrono::system_clock::now()) - p_->clientTime();
    state_.auth_code1 = p_->AuthCode();
    state_.aid = p_->AID();
    state_.cid = p_->GID();
    
    SPDLOG_TRACE(log(), "Sending ARES_ZH_CID_AUTH_REQ");
    uint32_t request_id;
    {
      std::lock_guard<std::mutex> lock(server_.auth_requests->mutex());
      request_id = server_.auth_requests->new_request(session_.shared_from_this());
    }
    server_.char_server()->emplace_and_send<packet::current<packet::ARES_ZH_CID_AUTH_REQ>>(request_id,
                                                                                           state_.aid,
                                                                                           state_.cid,
                                                                                           state_.auth_code1);
  }
  SPDLOG_TRACE(log(), "handle_packet CZ_ENTER: end");
}
