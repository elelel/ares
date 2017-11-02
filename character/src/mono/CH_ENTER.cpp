#include "packet_handlers.hpp"

#include <ares/common>

#include "state.hpp"
#include "../server.hpp"
#include "../client/state.hpp"

void ares::character::mono::packet_handler<ares::packet::CH_ENTER>::operator()() {
  SPDLOG_TRACE(log(), "CH_ENTER: begin");
  log()->info("CH_ENTER request AID = {}, AuthCode = {}, userLevel = {}, clientType = {}, Sex = {}",
              p_->AID(), p_->AuthCode(), p_->userLevel(), p_->clientType(), p_->Sex());
  SPDLOG_TRACE(log(), "Sending back AID = {} in response to CH_ENTER", p_->AID());
  const auto aid = p_->AID();
  session_.copy_and_send(&aid, sizeof(aid));
  SPDLOG_TRACE(log(), "CH_ENTER acquiring server lock");
  std::lock_guard<std::mutex> lock(server_.mutex());
  SPDLOG_TRACE(log(), "CH_ENTER acquiring server lock acquired");
  auto found = server_.client_by_aid(p_->AID());
  if (found &&
      (p_->AuthCode() == found->as_client().auth_code1) &&
      ((int32_t)p_->userLevel() == found->as_client().auth_code2)) {
    from_zone_server(found);
  } else {
    from_account_server();
  }
  SPDLOG_TRACE(log(), "CH_ENTER: end");
}

void ares::character::mono::packet_handler<ares::packet::CH_ENTER>::from_zone_server(session_ptr) {
  SPDLOG_TRACE(log(), "CH_ENTER - transforming from existing session {:#x}", (uintptr_t)existing_session.get());
  // TODO:
}

void ares::character::mono::packet_handler<ares::packet::CH_ENTER>::from_account_server() {
  SPDLOG_TRACE(log(), "Sending ATHENA_HA_AID_AUTH_REQ");
  server_.account_server()->emplace_and_send<packet::ATHENA_HA_AID_AUTH_REQ>(p_->AID(),
                                                                             p_->AuthCode(),
                                                                             (int32_t)p_->userLevel(),
                                                                             p_->Sex(),
                                                                             0,
                                                                             random_int32::get());
}

void ares::character::mono::packet_handler<ares::packet::CH_ENTER>::refuse(const uint8_t error_code) {
  session_.emplace_and_send<packet::HC_REFUSE_ENTER>(error_code);
  throw ares::network::terminate_session();
}

void ares::character::mono::packet_handler<ares::packet::CH_ENTER>::notify_ban(const uint8_t error_code) {
  session_.emplace_and_send<packet::SC_NOTIFY_BAN>(error_code); 
  throw ares::network::terminate_session();
}

