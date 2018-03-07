#pragma once

#include "session.hpp"

#include <ares/common/grf>

namespace ares {
  namespace character {
    struct server;
    
    struct maps_manager {
      maps_manager(std::shared_ptr<spdlog::logger> log,
                   const config::postgres_config& pg_conf,
                   const config::zone_server_config& zs_conf,
                   const std::vector<std::string>& grfs);
      void remove_zone_session(session_ptr s);

      uint32_t id_by_name(const std::string& map_name);
      const std::string& name_by_id(const uint32_t map_id);
      
    private:
      // Map name index
      std::map<uint32_t, std::string> map_id_to_name_;
      std::unordered_map<std::string, uint32_t> map_name_to_id_;
      std::map<uint32_t, session_ptr> id_to_zone_session_;

      /*! Configuration for zones - map id assignment
       */
      std::unordered_map<std::string, std::vector<uint32_t>> zone_login_to_id_;

      /*! Map ids that are already assigned to a map
       */
      std::set<uint32_t> assigned_ids_;

      std::shared_ptr<ares::grf::resource_set> resources_;

      void load_map_for_zone(const std::string& map_name, const std::string& zone_login);
      std::optional<uint32_t> load_and_verify(const std::string& map_name);
      uint32_t create_map(const std::string& map_name);
      std::shared_ptr<ares::grf::resource_set> resources();

      std::mutex mutex_;
      std::shared_ptr<spdlog::logger> log_;
      const std::vector<std::string>& grfs_;
    };
  }
}
