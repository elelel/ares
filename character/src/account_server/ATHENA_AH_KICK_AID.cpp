#include "state.hpp"
#include "../state.hpp"

void ares::character::account_server::packet_handler<ares::packet::ATHENA_AH_KICK_AID>::operator()() {
  SPDLOG_TRACE(log(), "ATHENA_AH_KICK_AID: begin");
  auto& server = server_state_.server;
  std::lock_guard<std::mutex> lock(server.mutex());
  auto s = server.client_by_aid(p_->aid());
  if (s != nullptr) {
    std::lock_guard<std::mutex> lock(server.mutex());
    s->remove_from_server();
    // TODO: Notify map servers to kick the user
  }
  SPDLOG_TRACE(log(), "ATHENA_AH_KICK_AID: end");
}
