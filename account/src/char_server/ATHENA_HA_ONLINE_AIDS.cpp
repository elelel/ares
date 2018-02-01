#include "state.hpp"
#include "../state.hpp"

void ares::account::char_server::packet_handler<ares::packet<ares::packets::ATHENA_HA_ONLINE_AIDS>>::operator()() {
  SPDLOG_TRACE(log(), "handle_packet ATHENA_HA_ONLINE_AIDS");
  const auto expected_min_sz = p_->users_sz() * sizeof(uint32_t) + sizeof(*p_);
  if (p_->PacketLength >= expected_min_sz) {
    SPDLOG_TRACE(log(), "ATHENA_HA_ONLINE_AIDS acquiring server lock");
    std::lock_guard lock(server_state_.server.mutex());
    SPDLOG_TRACE(log(), "ATHENA_HA_ONLINE_AIDS server lock acquired");
    for (size_t i = 0; i < p_->users_sz(); ++i) {
      server_state_.server.link_aid_to_char_server(p_->users()[i], session_.shared_from_this());
    }
    SPDLOG_TRACE(log(), "Linked aids to char servers");
  }
  else {
    log()->warn("Bad ATHENA_HA_ONLINE_AIDS packet: PacketLength = {} < expected_min_sz = {}", p_->PacketLength, expected_min_sz);
  }
  SPDLOG_TRACE(log(), "handle_packet ATHENA_HA_ONLINE_AIDS end");
}
