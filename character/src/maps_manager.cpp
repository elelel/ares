#include "maps_manager.hpp"

#include "server.hpp"

#include <thread>

#include <ares/database>

thread_local std::shared_ptr<ares::database::db> db = nullptr;

ares::character::maps_manager::maps_manager(std::shared_ptr<spdlog::logger> log,
                                            const config::postgres_config& pg_conf,
                                            const config::zone_server_config& zs_conf,
                                            const std::vector<std::string>& grfs) :
  log_(log),
  grfs_(grfs) {
  log_->info("Loading maps configuration and verifying maps");

  const auto cores = std::thread::hardware_concurrency();
  for (const auto& zs : zs_conf) {
    std::vector<std::string> maps(zs.maps.begin(), zs.maps.end());
    auto batch_sz = (maps.size() / cores) + ((maps.size() % cores) / cores) + 1;
    std::vector<std::thread> threads;
    for (size_t i = 0; i < cores; ++i) {
      size_t start = i * batch_sz;
      size_t end = ((i + 1) * batch_sz) > maps.size() ? maps.size() : (i + 1) * batch_sz;
      threads.push_back
        (std::thread([this, &pg_conf, &zs, &maps, start, end] () {
            db = std::make_shared<ares::database::db>(log_, pg_conf->dbname, pg_conf->host, pg_conf->port, pg_conf->user, pg_conf->password);
            for (size_t i = start; i < end; ++i) {
              load_map_for_zone(maps[i], zs.login);
            }
            db.reset();
          }));
    }
    for (size_t i = 0; i < cores; ++i) threads[i].join();
  }
  log_->info("Map verification done");

  log_->info("Loading maps index");
  db = std::make_shared<ares::database::db>(log_, pg_conf->dbname, pg_conf->host, pg_conf->port, pg_conf->user, pg_conf->password);
  auto known_maps = db->query<database::maps::ids_and_names>();
  for (const auto& m : known_maps) {
    map_name_to_id_[std::get<1>(m)] = std::get<0>(m);
    map_id_to_name_[std::get<0>(m)] = std::get<1>(m);
  }

  resources_ = nullptr;
}

void ares::character::maps_manager::remove_zone_session(std::shared_ptr<session> s) {
  for (auto it = id_to_zone_session_.begin(); it != id_to_zone_session_.end();) {
    const auto& r = *it;
    if (r.second.lock() == s) {
      assigned_ids_.erase(r.first);
      it = id_to_zone_session_.erase(it);
    } else {
      ++it;
    }
  }
}

void ares::character::maps_manager::load_map_for_zone(const std::string& map_name, const std::string& zone_login) {
  auto map_id = load_and_verify(map_name);
  if (!map_id) {
    log_->warn("Data for map '{}' is corrupt or missing, loading from dir/grf resources", map_name);
    map_id.emplace(create_map(map_name));
  }

  std::lock_guard<std::mutex> lock(mutex_);
  if (assigned_ids_.find(*map_id) == assigned_ids_.end()) {
    zone_login_to_id_[zone_login].push_back(*map_id);
    assigned_ids_.insert(*map_id);
  } else {
    auto msg = std::string("Can't assign map '") + map_name + "' to zone server '" + zone_login + "' because it has already been assigned";
    log_->error(msg);
    throw std::runtime_error(msg);
  }
}

std::optional<uint32_t> ares::character::maps_manager::load_and_verify(const std::string& map_name) {
  auto map_id = db->query<database::maps::id_by_name>(map_name);
  if (map_id) {
    auto map_info = db->query<database::maps::info>(*map_id);
    if (!map_info) {
      map_id.reset();
    } else {
      for (const auto& f : map_info->static_flags().container()) {
        if ((f.data() & ~(model::map_cell_flags::walkable | model::map_cell_flags::shootable | model::map_cell_flags::water)) != 0) {
          SPDLOG_TRACE(log_, "Unknown map_cell_flags {}", f.data());
          map_id.reset();
          break;
        }
      }
    }
  }
  return map_id;
}

uint32_t ares::character::maps_manager::create_map(const std::string& map_name) {
  auto gat_fn = resources()->find_resource_file(map_name + ".gat");
  if (!gat_fn) { gat_fn = "data\\" + map_name + ".gat"; };
  auto rsw_fn = resources()->find_resource_file(map_name + ".rsw");
  if (!rsw_fn) { rsw_fn = "data\\" + map_name + ".rsw"; };
  auto gat = resources()->read_file(*gat_fn);
  auto rsw = resources()->read_file(*rsw_fn);

  if (gat && rsw && (gat->size() > 0) && (rsw->size() > 0)) {
    const auto& g = *gat;
    const auto& r = *rsw;
    auto x_size = *(int32_t*)(&(g[6]));
    auto y_size = *(int32_t*)(&(g[10]));
    model::map_info mi(x_size, y_size);    
    const auto water_height = *(float*)(&(r[166]));
    size_t off{14};
    // TODO: Check x/y storage order in gat
    for (int x = 0; x < x_size; ++x) {
      for (int y = 0; y < y_size; ++y) {
        auto height = *(float*)(&(g[off]));
        auto type = *(uint32_t*)(&(g[off + 16]));
        if ((type == 0) && (height > water_height)) type = 3;
        mi.static_flags(x, y) = model::map_cell_flags(model::map_cell_gat_type::from_gat_uint32(type));
        off += 20;
      }
    }
    auto rslt = db->query<database::maps::update>(map_name, mi);
    if (rslt) return *rslt; else {
      log_->error("Failed to recreate map '" + map_name + "' in database");
      throw std::runtime_error("Failed to recreate map '" + map_name + "' in database");
    }
  } else {
    log_->error("Could not load gat/rsw for map '{}'", map_name);
    throw std::runtime_error("Could not load gat/rsw for '" + map_name +"'");
  }
}

auto ares::character::maps_manager::resources() -> std::shared_ptr<ares::grf::resource_set> {
  std::lock_guard<std::mutex> lock(mutex_);
  if (resources_ == nullptr) {
    try {
      log_->info("Reinitializing dir/grf resources...");
      resources_ = std::make_shared<ares::grf::resource_set>(grfs_);
      return resources_;
    } catch (std::runtime_error e) {
      log_->error("Failed to initialize dir/grf resources - {}", e.what());
      throw std::runtime_error(std::string("Failed to initialize dir/grf resources - ") + e.what());
    }
  } else {
    return resources_;
  }
}

uint32_t ares::character::maps_manager::id_by_name(const std::string& map_name) const {
  auto found = map_name_to_id_.find(map_name);
  if (found != map_name_to_id_.end()) {
    return found->second;
  }
  return 0;
}

const std::string& ares::character::maps_manager::name_by_id(const uint32_t map_id) const {
  static std::string empty_string;
  auto found = map_id_to_name_.find(map_id);
  if (found != map_id_to_name_.end()) {
    return found->second;
  }
  return empty_string;
}

auto ares::character::maps_manager::zone_session_by_id(const uint32_t map_id) const -> std::shared_ptr<session> {
  auto found = id_to_zone_session_.find(map_id);
  if (found != id_to_zone_session_.end()) {
    return found->second.lock();
  }
  return nullptr;
}

bool ares::character::maps_manager::link_id_to_zone_session(const uint32_t map_id, std::shared_ptr<session> s) {
  auto found = zone_session_by_id(map_id);
  if (found == nullptr) {
    id_to_zone_session_[map_id] = s;
    return true;
  } else {
    log_->error("Can't link map id {} to zone session with id {} because it is already linked to zone session with id {}",
                map_id, s->id(), found->id());
    return false;
  }
}

auto ares::character::maps_manager::assigned_to_zone_login(const std::string& login) const -> const std::vector<uint32_t>& {
  static const std::vector<uint32_t> empty;
  auto found = zone_login_to_id_.find(login);
  if (found != zone_login_to_id_.end()) return found->second;
  return empty;
}
