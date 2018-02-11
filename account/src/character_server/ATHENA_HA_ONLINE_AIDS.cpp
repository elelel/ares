#include "state.hpp"
#include "../server.hpp"

void ares::account::character_server::packet_handler<ares::packet::current<ares::packet::ATHENA_HA_ONLINE_AIDS>>::operator()() {
  SPDLOG_TRACE(log(), "handle_packet ATHENA_HA_ONLINE_AIDS");
  const auto expected_min_sz = p_->users_sz() * sizeof(uint32_t) + sizeof(*p_);
  if (p_->PacketLength >= expected_min_sz) {
    std::lock_guard<std::mutex> lock(server_.mutex());
    for (size_t i = 0; i < p_->users_sz(); ++i) {
      server_.link_aid_to_char_server(p_->users()[i], session_.shared_from_this());
      SPDLOG_TRACE(log(), "Linked aids to char servers");
    }
  }
  else {
    log()->warn("Bad ATHENA_HA_ONLINE_AIDS packet: PacketLength = {} < expected_min_sz = {}", p_->PacketLength, expected_min_sz);
  }
  SPDLOG_TRACE(log(), "handle_packet ATHENA_HA_ONLINE_AIDS end");
}
