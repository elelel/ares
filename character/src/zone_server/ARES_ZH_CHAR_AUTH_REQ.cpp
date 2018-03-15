#include "state.hpp"
#include "../server.hpp"

void ares::character::zone_server::packet_handler<ares::packet::current<ares::packet::ARES_ZH_CHAR_AUTH_REQ>>::operator()() {
  SPDLOG_TRACE(log(), "ARES_ZH_CHAR_AUTH_REQ: begin");
  std::shared_ptr<session> found;
  {
    std::lock_guard<std::mutex> lock(server_.mutex());
    found = server_.find_client_session(p_->account_id());
  }
  if (found) {
    const auto& data = found->as_client();
    if ((data.auth_code1 == p_->auth_code1()) && (data.account_id == p_->account_id())) {
      if (data.char_info) {
        if (data.char_info->character_id == p_->character_id()) {
          SPDLOG_TRACE(log(), "Authenticated account_id {}, character_id{} to zone (char name '{}', map id '{}')",
                       p_->account_id().to_string(), p_->character_id().to_string(), (const char*)data.char_info->name, data.char_info->location_last.map_id);
          session_.emplace_and_send<packet::current<packet::ARES_HZ_CHAR_AUTH_RESULT>>(p_->request_id(), p_->character_id(), data.auth_code2, 0);
        } else {
          log()->warn("Failed authentication attempt from zone active account_id {} with character_id {}, character_id mismatch", p_->account_id().to_string(), p_->character_id().to_string());
          session_.emplace_and_send<packet::current<packet::ARES_HZ_CHAR_AUTH_RESULT>>(p_->request_id(), p_->character_id(), 0, 1);
        }
      } else {
        log()->error("Char info is not loaded from database while authenticating account_id {} from zone with character_id {}, auth_code1 {:#x}",
                     p_->account_id().to_string(), p_->character_id().to_string(), p_->auth_code1());
        session_.emplace_and_send<packet::current<packet::ARES_HZ_CHAR_AUTH_RESULT>>(p_->request_id(), p_->character_id(), 0, 2);
      }
    } else {
      log()->warn("Failed authentication attempt from zone for active account_id. Packet/data: account_id {}/{}, character_id {}/{}, auth_code1 {:#x}/{:#x}",
                  p_->account_id().to_string(), data.account_id.to_string(),
                  p_->character_id().to_string(), data.char_info->character_id.to_string(),
                  p_->auth_code1(), data.auth_code1);
      session_.emplace_and_send<packet::current<packet::ARES_HZ_CHAR_AUTH_RESULT>>(p_->request_id(), p_->character_id(), 0, 3);
    }
  } else {
    log()->warn("Failed authentication attempt from zone for inactive account_id {} with character_id {}, auth_code1 {:#x}",
                p_->account_id().to_string(), p_->character_id().to_string(), p_->auth_code1());
    session_.emplace_and_send<packet::current<packet::ARES_HZ_CHAR_AUTH_RESULT>>(p_->request_id(), p_->character_id(), 0, 4);
  }
  SPDLOG_TRACE(log(), "ARES_ZH_CHAR_AUTH_REQ: end");
}
