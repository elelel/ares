#include "state.hpp"
#include "../state.hpp"

void ares::character::client::packet_handler<ares::packet<ares::packets::PING>>::operator()() {
  SPDLOG_TRACE(server_state_.log(), "PING from client, aid {} ", p_->aid());
}

