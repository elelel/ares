#include "packet_handlers.hpp"

#include "state.hpp"
#include "../server.hpp"

void ares::character::account_server::packet_handler<ares::packet::ATHENA_AH_KICK_AID>::operator()() {
  SPDLOG_TRACE(log(), "ATHENA_AH_KICK_AID: begin");
  std::lock_guard<std::mutex> lock(server_.mutex());
  auto s = server_.client_by_aid(p_->aid());
  if (s != nullptr) {
    std::lock_guard<std::mutex> lock(server_.mutex());
    s->remove_from_server();
    // TODO: Notify map servers to kick the user
  }
  SPDLOG_TRACE(log(), "ATHENA_AH_KICK_AID: end");
}
