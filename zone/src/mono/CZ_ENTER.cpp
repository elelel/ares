#include "state.hpp"

#include "../server.hpp"

void ares::zone::mono::packet_handler<ares::packet::current<ares::packet::CZ_ENTER>>::operator()() {
  SPDLOG_TRACE(log(), "handle_packet CZ_ENTER: begin");

  SPDLOG_TRACE(log(), "CZ_ENTER: AID = {}, GID = {}, AuthCode = {}, clientTime = {}, Sex = {}",
               p_->AID(), p_->GID(), p_->AuthCode(), p_->clientTime(), p_->Sex());

  // TODO: Check for existing connections
  
  // TODO: Post auth request to char server
  {
    // TODO: diff client time
    //     state_.client_time_diff = std::chrono::system_clock::now() - std::chrono::milliseconds(p_->clientTime());
    state_.auth_code1 = p_->AuthCode();
    state_.cid = p_->GID();
    
    std::lock_guard<std::mutex> lock(server_.mutex());
    SPDLOG_TRACE(log(), "Sending ATHENA_HA_AID_AUTH_REQ");
    uint32_t request_id;
    {
      std::lock_guard<std::mutex> lock(server_.mutex());
      request_id = server_.auth_requests->new_request(session_.shared_from_this());
    }
    server_.char_server()->emplace_and_send<packet::current<packet::ARES_ZH_CID_AUTH_REQ>>(request_id,
                                                                                           state_.cid,
                                                                                           state_.auth_code1);
  }

  
  // TODO: This should happen after char server has authorized the session
  if (server_.conf().obfuscation_key) {
    /*    state_.obf_crypt_key_.emplace(((std::get<0>(k) * std::get<1>(k) + std::get<2>(k)) *
          std::get<1>(k)) + std::get<2>(k));*/
    // CZ_ENTER requires additional round of obfuscation key rotation
    state_.rotate_obf_crypt_key();
  }

  SPDLOG_TRACE(log(), "handle_packet CZ_ENTER: end");
}
