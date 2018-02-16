#include <ares/common>

#include "state.hpp"
#include "../server.hpp"
#include "../client/state.hpp"

void ares::character::mono::packet_handler<ares::packet::current<ares::packet::CH_ENTER>>::operator()() {
  SPDLOG_TRACE(log(), "CH_ENTER: begin");
  log()->info("CH_ENTER request AID = {}, AuthCode = {}, userLevel = {}, clientType = {}, Sex = {}",
              p_->AID(), p_->AuthCode(), p_->userLevel(), p_->clientType(), p_->Sex());
  SPDLOG_TRACE(log(), "Sending back AID = {} in response to CH_ENTER", p_->AID());
  const auto aid = p_->AID();
  session_.copy_and_send(&aid, sizeof(aid));
  session_ptr client;
  {
    SPDLOG_TRACE(log(), "CH_ENTER acquiring server lock");
    std::lock_guard<std::mutex> lock(server_.mutex());
    SPDLOG_TRACE(log(), "CH_ENTER acquiring server lock acquired");
    client = server_.client_by_aid(p_->AID());
  }
  if (client &&
      (p_->AuthCode() == client->as_client().auth_code1) &&
      ((int32_t)p_->userLevel() == client->as_client().auth_code2)) {
    // TODO: from zone server
  } else {
    // Login from account server
    SPDLOG_TRACE(log(), "Sending ATHENA_HA_AID_AUTH_REQ");
    auto request_id = random_int32::get();
    {
      std::lock_guard<std::mutex> lock(server_.mutex());
      server_.add_auth_aid_request(request_id, session_.shared_from_this());
    }
    server_.account_server()->emplace_and_send<packet::current<packet::ATHENA_HA_AID_AUTH_REQ>>(p_->AID(),
                                                                                                p_->AuthCode(),
                                                                                                (int32_t)p_->userLevel(),
                                                                                                p_->Sex(),
                                                                                                0,
                                                                                                request_id);
    
  }
  SPDLOG_TRACE(log(), "CH_ENTER: end");
}

