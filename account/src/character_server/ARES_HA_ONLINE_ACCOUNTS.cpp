#include "state.hpp"
#include "../server.hpp"

void ares::account::character_server::packet_handler<ares::packet::current<ares::packet::ARES_HA_ONLINE_ACCOUNTS>>::operator()() {
  SPDLOG_TRACE(log(), "handle_packet ARES_HA_ONLINE_ACCOUNTS");
  const auto expected_min_sz = p_->count() * sizeof(model::account_id) + sizeof(*p_);
  if (p_->PacketLength >= expected_min_sz) {
    std::lock_guard<std::mutex> lock(server_.mutex());
    for (size_t i = 0; i < p_->count(); ++i) {
      server_.link_to_char_server(p_->account_ids()[i], session_.shared_from_this());
      SPDLOG_TRACE(log(), "Linked accounts to char servers");
    }
  }
  else {
    log()->warn("Bad ARES_HA_ONLINE_ACCOUNTS packet: PacketLength = {} < expected_min_sz = {}", p_->PacketLength, expected_min_sz);
  }
  SPDLOG_TRACE(log(), "handle_packet ARES_HA_ONLINE_ACCOUNTS end");
}
