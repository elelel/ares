#include "state.hpp"
#include "../server.hpp"

void ares::character::account_server::packet_handler<ares::packet::current<ares::packet::ATHENA_AH_KICK_AID>>::operator()() {
  SPDLOG_TRACE(log(), "ATHENA_AH_KICK_AID: begin");
  session_ptr s;
  {
    std::lock_guard<std::mutex> lock(server_.mutex());
    s = server_.client_by_aid(p_->aid());
  }
  if (s != nullptr) {
    // TODO: Notify map servers to kick the user
    s->close_gracefuly();
  }
  SPDLOG_TRACE(log(), "ATHENA_AH_KICK_AID: end");
}
