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

      bool link_id_to_zone_session(const uint32_t map_id, std::shared_ptr<session> s);
      std::shared_ptr<session> zone_session_by_id(const uint32_t map_id) const;
      void remove_zone_session(std::shared_ptr<session> s);
      const std::vector<uint32_t>& assigned_to_zone_login(const std::string& login) const;

      uint32_t id_by_name(const std::string& map_name) const;
      const std::string& name_by_id(const uint32_t map_id) const;

    private:
      // Map name index
      std::map<uint32_t, std::string> map_id_to_name_;
      std::unordered_map<std::string, uint32_t> map_name_to_id_;
      std::map<uint32_t, std::weak_ptr<session>> id_to_zone_session_;

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
