#pragma once

#include <ares/config>

namespace ares {
  namespace account {
    struct config : ares::config {
      config(std::shared_ptr<spdlog::logger> log,
             std::shared_ptr<asio::io_service> io_sevice,
             const std::optional<std::string> first_config_file);

      struct char_server_config_record {
        std::string login;
        std::string password;
      };
      using char_server_config = std::vector<char_server_config_record>;

      std::optional<size_t> network_threads{std::optional<size_t>(2)};
      postgres_config postgres;
      endpoints_config listen_ipv4;
      std::optional<uint32_t> client_version;
      char_server_config char_servers;
      
    private:
      void load_char_servers();
    };
    
  }
}
