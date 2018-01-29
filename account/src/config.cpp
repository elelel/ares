#include "config.hpp"

ares::account::config::config(std::shared_ptr<spdlog::logger> log,
                              std::shared_ptr<asio::io_service> io_service,
                              std::optional<std::string> first_config_file) :
  ares::config(log, io_service, "account_server", first_config_file) {
  postgres = load_with_catch_as<postgres_config>("database", json_);
  listen_ipv4 = load_with_catch_as<endpoints_config>("listen_ipv4", json_);
  load_char_servers();
}

void ares::account::config::load_char_servers() {
  auto load_char_servers = [this] () {
    auto j_char_servers = json_.find("char_servers");
    if ((j_char_servers != json_.end()) && (j_char_servers->is_array())) {
      for (const auto& j_char_server : *j_char_servers) {
        char_server_config_record r;
        if (j_char_server.find("login") != j_char_server.end())
          r.login = j_char_server.at("login");
        else log_->error("Char server config record doesn't contain 'login' field");
        if (j_char_server.find("password") != j_char_server.end())
          r.password = j_char_server.at("password");
        else log_->error("Char server config record doesn't contain 'password' field");
        char_servers.push_back(r);
      }
    } else {
      log_->warn("No character servers configured");
    }
  };
  with_catch("char_servers", load_char_servers);
}

