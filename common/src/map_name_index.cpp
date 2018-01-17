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

ares::map_name_index::map_name_index(std::shared_ptr<spdlog::logger> log, std::vector<std::string>&& source_names) :
  names_(std::move(source_names)),
  log_(log) {
  size_t i{0};
  for (const auto& n : names_) {
    name_to_idx_[n] = i;
    ++i;
  }
  log->info("map_name_index: loaded {} map names", names_.size());
}

void ares::map_name_index::load_json(const nlohmann::json& j) {
  auto map_names = j.find("map_names");
  if ((map_names != j.end()) && (map_names->is_array())) {
    size_t i{0};
    for (const auto& n : *map_names) {
      names_.push_back(n);
      name_to_idx_[n] = i;
      ++i;
    }
  } else {
    log_->error("map_name_index: 'map_names' array must exist in map name index list");
  }
  log_->info("map_name_index: loaded {} map names", names_.size());
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
  if (idx < names_.size()) {
    return names_[idx];
  } else {
    log_->error("map_name_index: queried index {} is out of bounds", idx);
    return empty_string;
  }
}
