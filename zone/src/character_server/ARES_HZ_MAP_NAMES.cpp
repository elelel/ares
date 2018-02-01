#include "state.hpp"
#include "../state.hpp"

void ares::zone::character_server::packet_handler<ares::packet<ares::packets::ARES_HZ_MAP_NAMES>>::operator()() {
  SPDLOG_TRACE(log(), "handle_packet ARES_HZ_MAP_NAMES: begin");
  if (p_->PacketLength != sizeof(packet<packets::ARES_HZ_MAP_NAMES>)) {
    const size_t max_i = p_->PacketLength - sizeof(packet<packets::ARES_HZ_MAP_NAMES>);
    for (size_t i = 0; i < max_i;) {
      std::string s((char*)&(p_->map_names()[i]));
      i += s.size() + 1;
      SPDLOG_TRACE(log(), "ARES_HZ_MAP_NAMES: character server assigned map '{}'", s);
      server_state_.map_names.insert(std::move(s));
    }
    // Request more map names
    session_.emplace_and_send<packet<packets::ARES_ZH_MAP_NAMES_REQ>>(session_.recv_unfragmented_free_size() / 2);
  } else {
    log()->info("Finished receiving map names from character server, total {}", server_state_.map_names.size());
    // No more map names, continue
  }
  SPDLOG_TRACE(log(), "handle_packet ARES_HZ_MAP_NAMES: end");
}
