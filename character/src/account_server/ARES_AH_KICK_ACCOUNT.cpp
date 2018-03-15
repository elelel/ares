#include "state.hpp"
#include "../server.hpp"

void ares::character::account_server::packet_handler<ares::packet::current<ares::packet::ARES_AH_KICK_ACCOUNT>>::operator()() {
  SPDLOG_TRACE(log(), "ARES_AH_KICK_ACCOUNT: begin");
  std::shared_ptr<session> s;
  {
    std::lock_guard<std::mutex> lock(server_.mutex());
    s = server_.find_client_session(p_->account_id());
  }
  if (s != nullptr) {
    // TODO: Notify map servers to kick the user
    s->close_gracefuly();
  }
  SPDLOG_TRACE(log(), "ARES_AH_KICK_ACCOUNT: end");
}
