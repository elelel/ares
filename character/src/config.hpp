#pragma once

#include <ares/config>

namespace ares {
  namespace character {
    struct config : ares::config {
      config(std::shared_ptr<spdlog::logger> log,
             std::shared_ptr<boost::asio::io_service> io_sevice,
             const std::optional<std::string> first_config_file);

      struct zone_server_config_record {
        std::string login;
        std::string password;
      };
      using zone_server_config = std::vector<zone_server_config_record>;

      struct account_server_config_record {
        endpoint_config connect;
        std::string login;
        std::string password;
      };
      using account_server_config = std::optional<account_server_config_record>;

      std::optional<postgres_config> postgres;
      endpoints_config listen_ipv4;
      std::optional<uint32_t> client_version;
      zone_server_config zone_servers;
      account_server_config account_server;
      std::optional<std::string> server_name;
      std::optional<std::string> priv_msg_server_name;
      
    private:
      void validate();
      void load_zone_servers();
      void load_account_server();
      void load_priv_msg_server_name();
      void load_server_name();
    };
    
  }
}
