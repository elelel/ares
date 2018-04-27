#include "state.hpp"

#include "../server.hpp"

void ares::zone::character_server::packet_handler<ares::packet::current<ares::packet::ARES_HZ_MAP_IDS>>::operator()() {
  SPDLOG_TRACE(log(), "handle_packet ARES_HZ_MAP_IDS: begin");
  if (p_->PacketLength != sizeof(handled_packet_type)) {
    {
      const size_t data_sz = p_->PacketLength - sizeof(handled_packet_type);
      const size_t batch_sz = data_sz / sizeof(uint32_t);

      std::lock_guard<std::mutex> lock(server_.maps.mutex());
      for (size_t i = 0; i < batch_sz; ++i) {
        auto map_id = p_->map_ids()[i];
        auto map_name = server_.maps.name_by_id(map_id);
        if (map_name != "") {
          auto map_info = server_.db->query<database::maps::info>(map_id);
          if (map_info) {
            auto m = std::make_shared<zone::map>(server_.log(), server_.io_context(), map_id, *map_info);
            server_.maps.add(map_id, m);
          } else {
            log()->error("No map information cached in database for map is {}. Reload maps database with character server.", map_id);
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
    SPDLOG_TRACE(log(), "Finished receiving map ids from character server");
  }
  SPDLOG_TRACE(log(), "handle_packet ARES_HZ_MAP_IDS: end");
}
