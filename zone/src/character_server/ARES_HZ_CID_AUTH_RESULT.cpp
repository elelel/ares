#include "state.hpp"
#include "../server.hpp"

void ares::zone::character_server::packet_handler<ares::packet::current<ares::packet::ARES_HZ_CID_AUTH_RESULT>>::operator()() {
  SPDLOG_TRACE(log(), "handle_packet ATHENA_HZ_CID_AUTH_RESULT: begin");
  std::shared_ptr<session> s;
  {
    std::lock_guard<std::mutex> lock(server_.mutex());
    s = server_.auth_requests->responded(p_->request_id());
  }
  if (s) {
    if (p_->result() == 0) {
      if (s->is_mono()) {
        server_.log()->info("Cid {} was authenticated by account server", p_->cid());
        auto new_state = s->as_mono();
        s->session_state_.emplace<client::state>(std::move(new_state));
        server_.add(s);
        // TODO: Continue ...
      } else {
        log()->error("Received authentication result from character server for cid {}, but it's session is not in mono state, ignoring.", p_->cid());
      }
    } else {
      log()->warn("Cid {} authentication denied by account server, closing session!", p_->cid());
      s->close_gracefuly();
    }
  } else {
    log()->error("Received authentication result from character server for cid {}, but auth request manager was not expecting it, ignoring.", p_->cid());
  }
  SPDLOG_TRACE(log(), "handle_packet ATHENA_HZ_CID_AUTH_RESULT: end");
}
