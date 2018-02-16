#include "state.hpp"

#include "../server.hpp"

void ares::zone::mono::packet_handler<ares::packet::current<ares::packet::CZ_ENTER>>::operator()() {
  SPDLOG_TRACE(log(), "handle_packet CZ_ENTER: begin");

  SPDLOG_TRACE(log(), "CZ_ENTER: AID = {}, GID = {}, AuthCode = {}, clientTime = {}, Sex = {}",
               p_->AID(), p_->GID(), p_->AuthCode(), p_->clientTime(), p_->Sex());

  // TODO: Post auth request to char server
  
  // TODO: This should happen after char server has authorized the session
  if (server_.conf().obfuscation_key) {
    /*    state_.obf_crypt_key_.emplace(((std::get<0>(k) * std::get<1>(k) + std::get<2>(k)) *
          std::get<1>(k)) + std::get<2>(k));*/
    // CZ_ENTER requires additional round of obfuscation key rotation
    state_.rotate_obf_crypt_key();
  }

  SPDLOG_TRACE(log(), "handle_packet CZ_ENTER: end");
}
