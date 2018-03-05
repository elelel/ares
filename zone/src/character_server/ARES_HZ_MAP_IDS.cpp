#include "state.hpp"

#include "../server.hpp"

void ares::zone::character_server::packet_handler<ares::packet::current<ares::packet::ARES_HZ_MAP_IDS>>::operator()() {
  SPDLOG_TRACE(log(), "handle_packet ARES_HZ_MAP_IDS: begin");
  if (p_->PacketLength != sizeof(handled_packet_type)) {
    {

      const size_t data_sz = p_->PacketLength - sizeof(handled_packet_type);
      const size_t batch_sz = data_sz / sizeof(uint32_t);
      std::shared_ptr<ares::grf::resource_set> resources;
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
          auto map_info = server_.db.map_info(map_id);
          if (!map_info || (uint32_t(map_info->x_size * map_info->y_size) != map_info->cell_flags.size())) {
            log()->warn("Information for map with name '{}' (id {}) does not exist in database cache or corrupt, loading from GRF", map_name, map_id);
            if (server_.grf_resources == nullptr) {
              try {
                log()->info("Reinitializing resources...");
                server_.grf_resources = std::make_shared<ares::grf::resource_set>(server_.conf().grfs);
              } catch (std::runtime_error e) {
                log()->error("Failed to initialize resources - {}", e.what());
                session_.close_gracefuly();
              }
            }
            auto res = server_.grf_resources;
            SPDLOG_TRACE(log(), "Loading {}.gat", map_name);
            auto gat_fn = res->find_resource_file(map_name + ".gat");
            if (!gat_fn) { gat_fn = "data\\" + map_name + ".gat"; };
            auto rsw_fn = res->find_resource_file(map_name + ".rsw");
            if (!rsw_fn) { rsw_fn = "data\\" + map_name + ".rsw"; };
            auto gat = res->read_file(*gat_fn);
            auto rsw = res->read_file(*rsw_fn);
            if (gat && rsw && (gat->size() > 0) && (rsw->size() > 0)) {
              SPDLOG_TRACE(log(), "Loaded {} - {}", gat->size(), rsw->size());
              // ... Make data and save to sql
            } else {
              log()->error("Could not load gat/rsw for map id {} ({})", map_id, map_name);
              if (gat->size() == 0) throw std::runtime_error("XXXXX");
              session_.close_gracefuly();
            }
          } 
          // ... Use the data
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
