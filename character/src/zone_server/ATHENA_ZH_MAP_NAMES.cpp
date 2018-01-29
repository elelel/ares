#include "state.hpp"
#include "../state.hpp"

void ares::character::zone_server::packet_handler<ares::packet::ATHENA_ZH_MAP_NAMES>::operator()() {
  SPDLOG_TRACE(server_state_.log(), "ATHENA_ZH_MAP_NAMES: begin, PacketLength = {}", p_->PacketLength);
  if (p_->PacketLength >= 4) {
    const size_t map_count = (p_->PacketLength - sizeof(*p_)) / sizeof(uint32_t);
    log()->info("Received {} maps form zone server", map_count);
    auto& map_ids = session_.as_zone_server().map_ids;
    map_ids.clear();
    map_ids.reserve(map_count);
    for (size_t i = 0; i < map_count; ++i) {
      map_ids.push_back(p_->map_ids()[i]);
    }
  } else {
    log()->warn("Received ATHENA_ZH_MAP_NAMES packet with no map ids");
  }
  SPDLOG_TRACE(server_state_.log(), "ATHENA_ZH_MAP_NAMES: end");
}
