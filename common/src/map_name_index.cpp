#include "map_name_index.hpp"

#include <fstream>

ares::map_name_index::map_name_index(std::shared_ptr<spdlog::logger> log, const nlohmann::json& j) :
  log_(log) {
  load_json(j);
}

ares::map_name_index::map_name_index(std::shared_ptr<spdlog::logger> log, const std::string& filename) :
  log_(log) {
  std::ifstream file;
  file.open(filename);
  if (file.is_open()) {
    if (file.good()) {
      nlohmann::json j;
      file >> j;
      load_json(j);
    } else {
      log_->error("map_name_index: file stream error after open (file {})", filename);
    }
  } else {
    log_->error("map_name_index: can't open map name index list file {}", filename);
    throw std::runtime_error("map_name_index: can't open map name index list file");
  }
}

ares::map_name_index::map_name_index(std::shared_ptr<spdlog::logger> log, std::unordered_map<std::string, size_t>&& source_names) :
  name_to_idx_(std::move(source_names)),
  log_(log) {
  for (const auto& n : name_to_idx_) {
    idx_to_name_.insert({n.second, n.first});
  }
  log->info("map_name_index: loaded {} map names", name_to_idx_.size());
}

void ares::map_name_index::load_json(const nlohmann::json& j) {
  auto map_names = j.find("map_names");
  if ((map_names != j.end()) && (map_names->is_array())) {
    for (const auto& p : *map_names) {
      auto kv = p.get<std::pair<std::string, size_t>>();
      name_to_idx_.insert(kv);
      idx_to_name_.insert({kv.second, kv.first});
    }
  } else {
    log_->error("map_name_index: 'map_names' array must exist in map name index list");
  }
  log_->info("map_name_index: loaded {} map names", name_to_idx_.size());
}

std::optional<size_t> ares::map_name_index::find(const std::string& n) const {
  auto found = name_to_idx_.find(n);
  if (found != name_to_idx_.end()) return std::optional<size_t>(found->second);
  else return std::optional<size_t>();
}

std::optional<size_t> ares::map_name_index::find(const char* n) const {
  auto found = name_to_idx_.find(n);
  if (found != name_to_idx_.end()) return std::optional<size_t>(found->second);
  else return std::optional<size_t>();
}

const std::string& ares::map_name_index::operator[](const size_t idx) const {
  static std::string empty_string{};
  auto found = idx_to_name_.find(idx);
  if (found != idx_to_name_.end()) {
    return found->second;
  } else {
    log_->error("map_name_index: queried index {} not found", idx);
    return empty_string;
  }
}
