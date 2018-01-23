#include "packet_handlers.hpp"

#include "state.hpp"
#include "../server.hpp"

void ares::character::account_server::packet_handler<ares::packet::ATHENA_AH_AID_AUTH_RESULT>::operator()() {
  SPDLOG_TRACE(log(), "ATHENA_AH_AID_AUTH_RESULT: begin");
  session_ptr s;
  {
    std::lock_guard<std::mutex> lock(server_.mutex());
    s = server_.session_by_auth_request_id(p_->request_id());
  }
  if (s != nullptr) {
    if (p_->fail() == 0) {
      if (std::holds_alternative<mono::state>(s->state_)) {
        log()->info("Aid {} was authenticated by account server", p_->aid());
        {
          std::lock_guard<std::mutex> lock(server_.mutex());
          server_.remove_auth_aid_request(p_->request_id());
          auto new_state = client::state(std::get<mono::state>(s->state_));
          s->state_.emplace<client::state>(std::move(new_state));
          auto& c = s->as_client();
          c.aid = p_->aid();
          c.auth_code1 = p_->auth_code1();
          c.auth_code2 = p_->auth_code2();
          server_.add(s);
        }
        session_.emplace_and_send<packet::ATHENA_HA_ACCOUNT_DATA_REQ>(p_->aid());
      } else {
        log()->error("Received authentication result from account server for aid {}, but it's session is not in mono state!", p_->aid());
        std::lock_guard<std::mutex> lock(server_.mutex());
        server_.remove(s);
      }
    } else {
      log()->warn("Aid {} is not authenticated by account server, closing session!", p_->aid());
      s->emplace_and_send<packet::AC_REFUSE_LOGIN>(8); // Rejected by server
      std::lock_guard<std::mutex> lock(server_.mutex());
      server_.remove(s);
    }
  } else {
    log()->error("ATHENA_AH_AID_AUTH_RESULT account server rejected login request. Check login/password configuration");
    throw network::terminate_session();
  }
  SPDLOG_TRACE(log(), "ATHENA_AH_AID_AUTH_RESULT: end");
}
