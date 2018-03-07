#include "state.hpp"

#include "../server.hpp"

void ares::zone::character_server::packet_handler<ares::packet::current<ares::packet::ARES_HZ_MAP_IDS>>::operator()() {
  SPDLOG_TRACE(log(), "handle_packet ARES_HZ_MAP_IDS: begin");
  if (p_->PacketLength != sizeof(handled_packet_type)) {
    {

      const size_t data_sz = p_->PacketLength - sizeof(handled_packet_type);
      const size_t batch_sz = data_sz / sizeof(uint32_t);
      for (size_t i = 0; i < batch_sz; ++i) {
        auto map_id = p_->map_ids()[i];
        std::string map_name;
        {
          std::lock_guard<std::mutex> lock(server_.mutex());
          auto found_name = server_.map_id_to_name.find(map_id);
          if (found_name != server_.map_id_to_name.end()) {
            map_name = found_name->second;
          }
        }
        
        if (map_name != "") {
          auto map_info = server_.db->query<database::maps::info>(map_id);
          if (map_info) {
          // ... Use the data
          } else {
            log()->error("No map information cached in database formap is {}. Reload maps database with character server.", map_id);
            session_.close_gracefuly();
          }
        } else {
          log()->error("Map id {} is not known to map index", map_id);
          session_.close_gracefuly();
        }
      }
    }
    // Request more map names
    session_.emplace_and_send<packet::current<packet::ARES_ZH_MAP_IDS_REQ>>(32 * 1024);
  } else {
    SPDLOG_TRACE(log(), "Finished receiving map names from character server, total {}", server_.map_ids.size());
    // No more map names, continue
  }
  SPDLOG_TRACE(log(), "handle_packet ARES_HZ_MAP_IDS: end");
}
