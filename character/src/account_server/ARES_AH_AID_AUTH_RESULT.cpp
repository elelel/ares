#include "state.hpp"
#include "../server.hpp"

void ares::character::account_server::packet_handler<ares::packet::current<ares::packet::ARES_AH_AID_AUTH_RESULT>>::operator()() {
  SPDLOG_TRACE(log(), "ARES_AH_AID_AUTH_RESULT: begin");
  session_ptr s;
  {
    std::lock_guard<std::mutex> lock(server_.mutex());
    s = server_.auth_requests->responded(p_->request_id());
  }
  if (s) {
    if (p_->result() == 0) {
      if (s->is_mono()) {
        server_.log()->info("Aid {} was authenticated by account server", p_->aid());
        auto new_state = s->as_mono();
        s->session_state_.emplace<client::state>(std::move(new_state));
        auto& c = s->as_client();
        c.aid = p_->aid();
        server_.add(s);
        session_.emplace_and_send<packet::current<packet::ATHENA_HA_ACCOUNT_DATA_REQ>>(p_->aid());
      } else {
        log()->error("Received authentication result from account server for aid {}, but it's session is not in mono state, ignoring.", p_->aid());
      }
    } else {
      log()->warn("Aid {} authentication denied by account server, closing session!", p_->aid());
      s->emplace_and_send<packet::current<packet::AC_REFUSE_LOGIN>>(8); // Rejected by server
      s->close_gracefuly();
    }
  } else {
    log()->error("Received authentication result from account server for aid {}, but auth request manager was not expecting it, ignoring.", p_->aid());
  }
}
