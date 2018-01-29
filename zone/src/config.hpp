#pragma once

#include <ares/config>

namespace ares {
  namespace zone {
    struct config : ares::config {
      config(std::shared_ptr<spdlog::logger> log,
             std::shared_ptr<asio::io_service> io_sevice,
             const std::optional<std::string> first_config_file);

      struct character_server_config_record {
        endpoint_config connect;
        std::string login;
        std::string password;
      };
      using character_server_config = std::optional<character_server_config_record>;

      std::optional<size_t> network_threads{std::optional<size_t>(2)};  // TODO: Read number of network threads from config
      std::optional<postgres_config> postgres;
      endpoints_config listen_ipv4;
      character_server_config character_server;
      
    private:
      void validate();
      void load_character_server();
    };
    
  }
}
