#include "state.hpp"

#include "../server.hpp"

#include <ares/common/grf>

void ares::zone::character_server::packet_handler<ares::packet::current<ares::packet::ARES_HZ_MAP_IDS>>::operator()() {
  SPDLOG_TRACE(log(), "handle_packet ARES_HZ_MAP_IDS: begin");
  if (p_->PacketLength != sizeof(handled_packet_type)) {
    {
      std::lock_guard<std::mutex> lock(server_.mutex());
      const size_t data_sz = p_->PacketLength - sizeof(handled_packet_type);
      const size_t batch_sz = data_sz / sizeof(uint32_t);
      std::shared_ptr<ares::grf::resource_set> resources;
      for (size_t i = 0; i < batch_sz; ++i) {
        auto map_id = p_->map_ids()[i];
        auto map_info = server_.db.map_info(map_id);
        if (!map_info || (uint32_t(map_info->x_size * map_info->y_size) != map_info->cell_flags.size())) {
          auto found = server_.map_id_to_name.find(map_id);
          std::string name;
          if (found != server_.map_id_to_name.end()) {
            name = "'" + found->second + "'";
          } else {
            name = "not known to map index";
          }
          log()->warn("Information for map with name {} (id {}) does not exist in database cache or corrupt, loading from GRF", name, map_id);
          if (resources == nullptr) {
            try {
              log()->info("Reinitializing resources...");
              resources = std::make_shared<ares::grf::resource_set>(server_.conf().grfs, *server_.conf().grf_resnametable_idx);
            } catch (std::runtime_error e) {
              log()->error("Failed to load resources - {}", e.what());
              throw;
            } catch (...) {
              throw;
            }
          }

          // TODO: Create map_info from grf, save to sql
        }
        // TODO: Create map
        server_.map_ids.insert(map_id);
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
