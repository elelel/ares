#pragma once

#include <map>
#include <unordered_map>
#include <string>

#include <spdlog/spdlog.h>
#include <src/json.hpp>

namespace ares {
  struct map_name_index {

    map_name_index(std::shared_ptr<spdlog::logger> log,
                   const nlohmann::json& json);
    map_name_index(std::shared_ptr<spdlog::logger> log,
                   const std::string& filename);
    map_name_index(std::shared_ptr<spdlog::logger> log,
                   std::unordered_map<std::string, size_t>&& source_names);

    std::optional<size_t> find(const std::string& n) const;
    std::optional<size_t> find(const char* n) const;
    const std::string& operator[](const size_t idx) const;
  private:
    std::map<size_t, std::string> idx_to_name_;
    std::unordered_map<std::string, size_t> name_to_idx_;
    std::shared_ptr<spdlog::logger> log_;

    void load_json(const nlohmann::json& json);
  };
}
