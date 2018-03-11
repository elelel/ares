#pragma once

#include <spdlog/spdlog.h>

#include "config.hpp"
#include "map.hpp"

namespace ares {
  namespace zone {
    struct maps_manager {
      maps_manager(std::shared_ptr<spdlog::logger> log,
                   const config::postgres_config& pg_conf);

      std::mutex& mutex();

      void add(const uint32_t map_id, std::shared_ptr<zone::map> m);
      
      std::shared_ptr<zone::map> by_id(const uint32_t map_id) const;

      uint32_t id_by_name(const std::string& map_name) const;
      const std::string& name_by_id(const uint32_t map_id) const;

    private:
      // Map name index
      std::map<uint32_t, std::string> map_id_to_name_;
      std::unordered_map<std::string, uint32_t> map_name_to_id_;

      // Main maps holder
      std::map<uint32_t, std::shared_ptr<zone::map>> maps_;

      std::mutex mutex_;
      std::shared_ptr<spdlog::logger> log_;
    };
  }
}
