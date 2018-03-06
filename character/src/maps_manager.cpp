#include "maps_manager.hpp"

#include "server.hpp"

#include <thread>

ares::character::maps_manager::maps_manager(ares::character::server& serv) :
  server_(serv) {
  server_.log()->info("Loading maps configuration");

  auto& conf = server_.conf();

  const auto cores = std::thread::hardware_concurrency();
  for (const auto& zs : conf.zone_servers) {
    std::vector<std::string> maps(zs.maps.begin(), zs.maps.end());
    auto batch_sz = (maps.size() / cores) + ((maps.size() % cores) / cores) + 1;
    std::vector<std::thread> threads;
    for (size_t i = 0; i < cores; ++i) {
      size_t start = i * batch_sz;
      size_t end = ((i + 1) * batch_sz) > maps.size() ? maps.size() : (i + 1) * batch_sz;
      threads.push_back(std::thread([this, &zs, &maps, start, end] () {
          for (size_t i = start; i < end; ++i) load_map_for_zone(maps[i], zs.login);          
          }));
    }
    for (size_t i = 0; i < cores; ++i) threads[i].join();
  }

  server_.log()->info("Loading maps index");
  auto known_maps = server_.db.map_name_index();
  for (const auto& m : known_maps) {
    map_name_to_id_[m.name] = m.id;
    map_id_to_name_[m.id] = m.name;
  }

  resources_ = nullptr;
}

void ares::character::maps_manager::remove_zone_session(session_ptr s) {
  for (auto it = id_to_zone_session_.begin(); it != id_to_zone_session_.end();) {
    const auto& r = *it;
    if (r.second == s) {
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
    server_.log()->warn("Information for map '{}' is corrupt or doesn't exist in database, loading from dir/grf resources", map_name);
    map_id.emplace(create_map(map_name));
  } else {
    server_.log()->info("Verified map '{}' id {}", map_name, *map_id);    
  }

  std::lock_guard<std::mutex> lock(mutex_);
  if (assigned_ids_.find(*map_id) == assigned_ids_.end()) {
    zone_login_to_id_[zone_login].push_back(*map_id);
    assigned_ids_.insert(*map_id);
  } else {
    auto msg = std::string("Can't assign map '") + map_name + "' to zone server '" + zone_login + "' because it has already been assigned";
    server_.log()->error(msg);
    throw std::runtime_error(msg);
  }
}

std::optional<uint32_t> ares::character::maps_manager::load_and_verify(const std::string& map_name) {
  auto map_id = server_.db.map_id_by_name(map_name);
  if (map_id) {
    auto map_info = server_.db.map_info(*map_id);
    if (!map_info || (uint32_t(map_info->x_size * map_info->y_size) != map_info->cell_flags.size())) {
      map_id.reset();
    } else {
      for (const auto& f : map_info->cell_flags) {
        if ((f.data() & ~(model::map_cell_flags::walkable | model::map_cell_flags::shootable | model::map_cell_flags::water)) != 0) {
          SPDLOG_TRACE(server_.log(), "Unknown map_cell_flags {}", f.data());
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
  model::map_info mi;
  if (gat && rsw && (gat->size() > 0) && (rsw->size() > 0)) {
    const auto& g = *gat;
    const auto& r = *rsw;
    mi.x_size = *(int32_t*)(&(g[6]));
    mi.y_size = *(int32_t*)(&(g[10]));
    const auto water_height = *(float*)(&(r[166]));
    const size_t xy_size = mi.x_size * mi.y_size;
    size_t off{14};
    for (size_t xy = 0; xy < xy_size; ++xy) {
      auto height = *(float*)(&(g[off]));
      auto type = *(uint32_t*)(&(g[off + 16]));
      if ((type == 0) && (height > water_height)) type = 3;
      mi.cell_flags.push_back(model::map_cell_flags(model::map_cell_gat_type::from_gat_uint32(type)));
      off += 20;
    }
    auto rslt = server_.db.recreate_map(map_name, mi);
    if (rslt) return *rslt; else {
      server_.log()->error("Failed to recreate map '" + map_name + "' in database");
      throw std::runtime_error("Failed to recreate map '" + map_name + "' in database");
    }
  } else {
    server_.log()->error("Could not load gat/rsw for map '{}'", map_name);
    throw std::runtime_error("Could not load gat/rsw for '" + map_name +"'");
  }
}

auto ares::character::maps_manager::resources() -> std::shared_ptr<ares::grf::resource_set> {
  std::lock_guard<std::mutex> lock(mutex_);
  if (resources_ == nullptr) {
    try {
      server_.log()->info("Reinitializing dir/grf resources...");
      resources_ = std::make_shared<ares::grf::resource_set>(server_.conf().grfs);
      return resources_;
    } catch (std::runtime_error e) {
      server_.log()->error("Failed to initialize dir/grf resources - {}", e.what());
      throw std::runtime_error(std::string("Failed to initialize dir/grf resources - ") + e.what());
    }
  } else {
    return resources_;
  }
}

uint32_t ares::character::maps_manager::id_by_name(const std::string& map_name) {
  auto found = map_name_to_id_.find(map_name);
  if (found != map_name_to_id_.end()) {
    return found->second;
  }
  return 0;
}

const std::string& ares::character::maps_manager::name_by_id(const uint32_t map_id) {
  static std::string empty_string;
  auto found = map_id_to_name_.find(map_id);
  if (found != map_id_to_name_.end()) {
    return found->second;
  }
  return empty_string;
}
