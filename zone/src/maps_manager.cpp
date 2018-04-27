#include "maps_manager.hpp"

#include <ares/database>

ares::zone::maps_manager::maps_manager(std::shared_ptr<spdlog::logger> log,
                                       const config::postgres_config& pg_conf) :
  log_(log) {
  log_->info("Loading maps index");
  auto db = std::make_shared<ares::database::db>(log_, pg_conf->dbname, pg_conf->host, pg_conf->port, pg_conf->user, pg_conf->password);
  auto known_maps = db->query<database::maps::ids_and_names>();
  for (const auto& m : known_maps) {
    map_name_to_id_[std::get<1>(m)] = std::get<0>(m);
    map_id_to_name_[std::get<0>(m)] = std::get<1>(m);
  }
}

std::mutex& ares::zone::maps_manager::mutex() {
  return mutex_;
}

void ares::zone::maps_manager::add(const uint32_t map_id, std::shared_ptr<zone::map> m) {
  if (maps_.find(map_id) == maps_.end()) {
    maps_[map_id] = m;
  } else {
    log_->warn("Map with id {} is already added to the server, not adding", map_id);
  }
}
      
auto ares::zone::maps_manager::by_id(const uint32_t map_id) const -> std::shared_ptr<zone::map> {
  auto found = maps_.find(map_id);
  if (found != maps_.end()) {
    return found->second;
  }
  return nullptr;
}

uint32_t ares::zone::maps_manager::id_by_name(const std::string& map_name) const {
  auto found = map_name_to_id_.find(map_name);
  if (found != map_name_to_id_.end()) {
    return found->second;
  }
  return 0;
}

const std::string& ares::zone::maps_manager::name_by_id(const uint32_t map_id) const {
  static std::string empty_string;
  auto found = map_id_to_name_.find(map_id);
  if (found != map_id_to_name_.end()) {
    return found->second;
  }
  return empty_string;
}
