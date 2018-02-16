#include "state.hpp"

#include "../server.hpp"

void ares::zone::character_server::packet_handler<ares::packet::current<ares::packet::ARES_HZ_MAP_NAMES>>::operator()() {
  SPDLOG_TRACE(log(), "handle_packet ARES_HZ_MAP_NAMES: begin");
  if (p_->PacketLength != sizeof(handled_packet_type)) {
    {
      std::lock_guard<std::mutex> lock(server_.mutex());
      const size_t max_i = p_->PacketLength - sizeof(handled_packet_type);
      for (size_t i = 0; i < max_i;) {
        std::string s((char*)&(p_->map_names()[i]));
        i += s.size() + 1;
        if (server_.map_names.find(s) == server_.map_names.end()) {
          server_.map_names.insert(std::move(s));
          SPDLOG_TRACE(log(), "ARES_HZ_MAP_NAMES: character server assigned map '{}'", s);
        }
      }
    }
    // Request more map names
    session_.emplace_and_send<packet::current<packet::ARES_ZH_MAP_NAMES_REQ>>(32 * 1024);
  } else {
    SPDLOG_TRACE(log(), "Finished receiving map names from character server, total {}", server_.map_names.size());
    // No more map names, continue
  }
  SPDLOG_TRACE(log(), "handle_packet ARES_HZ_MAP_NAMES: end");
}
