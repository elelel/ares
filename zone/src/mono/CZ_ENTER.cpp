#include "state.hpp"

#include "../state.hpp"
#include "../event/cz_enter.hpp"

void ares::zone::mono::packet_handler<ares::packet_set, ares::packet::CZ_ENTER>::operator()() {
  SPDLOG_TRACE(log(), "handle_packet CZ_ENTER: begin");

  SPDLOG_TRACE(log(), "CZ_ENTER: AID = {}, GID = {}, AuthCode = {}, clientTime = {}, Sex = {}",
               p_->AID(), p_->GID(), p_->AuthCode(), p_->clientTime(), p_->Sex());
  server_state_.post_event(event::cz_enter::from_packet(*p_));
  
  // TODO: This should happen after char server has authorized the session
  const auto& conf = session_.server_state_.conf;
  if (conf.obfuscation_key) {
    const auto& k = *conf.obfuscation_key;
    session_.obf_crypt_key.emplace(((std::get<0>(k) * std::get<1>(k) + std::get<2>(k)) *
                                    std::get<1>(k)) + std::get<2>(k));
  }

  SPDLOG_TRACE(log(), "handle_packet CZ_ENTER: end");
}
