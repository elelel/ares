#pragma once

#include <unordered_set>

#include <ares/config>

namespace ares {
  namespace character {
    struct config : ares::config {
      config(std::shared_ptr<spdlog::logger> log,
             std::shared_ptr<asio::io_service> io_sevice,
             const std::optional<std::string> first_config_file);

      struct zone_server_config_record {
        std::string login;
        std::string password;
        std::unordered_set<std::string> maps;
      };
      using zone_server_config = std::vector<zone_server_config_record>;

      struct account_server_config_record {
        endpoint_config connect;
        std::string login;
        std::string password;
      };
      using account_server_config = std::optional<account_server_config_record>;
      std::optional<size_t> network_threads{std::optional<uint32_t>(2)}; // TODO: read from config
      postgres_config postgres;
      endpoints_config listen_ipv4;
      std::optional<uint32_t> client_version;
      zone_server_config zone_servers;
      account_server_config account_server;
      std::optional<std::string> server_name;
      std::optional<std::string> priv_msg_server_name;
      std::vector<std::string> grfs;
      void load_grfs();
      
      // Server level character parameters used in char creation
      // TODO: Check which slots limits are not needed, convert to optional
      uint8_t normal_slots;
      uint8_t premium_slots;
      uint8_t billing_slots;
      uint8_t creatable_slots;
      uint8_t playable_slots;
      uint32_t bank_vault;
      uint32_t max_storage;
      
    private:
      void validate();
      void load_zone_servers();
      void load_account_server();
      void load_priv_msg_server_name();
      void load_server_name();
      void load_normal_slots();
      void load_premium_slots();
      void load_billing_slots();
      void load_creatable_slots();
      void load_playable_slots();
      void load_bank_vault();
      void load_max_storage();
    };
    
  }
}
