#include "state.hpp"

#include "../state.hpp"

void ares::zone::mono::packet_handler<ares::packet_set, ares::packet::CZ_ENTER>::operator()() {
  SPDLOG_TRACE(log(), "handle_packet CZ_ENTER: begin");

  SPDLOG_TRACE(log(), "CZ_ENTER: AID = {}, GID = {}, AuthCode = {}, clientTime = {}, Sex = {}",
               p_->AID(), p_->GID(), p_->AuthCode(), p_->clientTime(), p_->Sex());

  
  SPDLOG_TRACE(log(), "handle_packet CZ_ENTER: end");
}
