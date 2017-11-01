#include "config.hpp"

ares::zone::config::config(std::shared_ptr<spdlog::logger> log,
                              std::shared_ptr<boost::asio::io_service> io_service,
                              std::optional<std::string> first_config_file) :
  ares::config(log, io_service, "zone_server", first_config_file) {
  postgres = load_with_catch_as<postgres_config>("database", json_);
  listen_ipv4 = load_with_catch_as<endpoints_config>("listen_ipv4", json_);
  load_character_server();
  validate();
}

void ares::zone::config::validate() {
  bool need_comma = false;
  std::string msg;
  if (!character_server) {
    if (need_comma) msg += ", ";
    msg += "character server is missing";
    need_comma = true;
  }

  if (msg.size() > 0) {
    msg = "Configuration check failed: " + msg;
    log_->error(msg);
    throw std::runtime_error(msg);
  }
}


void ares::zone::config::load_character_server() {
  auto load_character_server = [this] () {
    auto j_character_server = json_.find("character_server");
    if ((j_character_server != json_.end()) && (j_character_server->is_object())) {
      character_server_config_record r;
      bool ok{true};
      if (j_character_server->find("connect") != j_character_server->end()) {
        r.connect = load<endpoint_config>(j_character_server->at("connect"));
      } else {
        ok = false;
        log_->error("Character server config record doesn't containt 'connect' field");
      }
      if (j_character_server->find("login") != j_character_server->end())
        r.login = j_character_server->at("login");
      else {
        ok = false;
        log_->error("Character server config record doesn't contain 'login' field");
      }
      if (j_character_server->find("password") != j_character_server->end())
        r.password = j_character_server->at("password");
      else {
        ok = false;
        log_->error("Character server config record doesn't contain 'password' field");
      }
      if (ok) character_server.emplace(r);
    } else {
      log_->warn("No character server configured");
    }
  };
  with_catch("character_server", load_character_server);
}
