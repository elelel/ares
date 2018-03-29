#include "config.hpp"

#include <sstream>

ares::zone::config::config(std::shared_ptr<spdlog::logger> log,
                              std::shared_ptr<asio::io_context> io_context,
                              std::optional<std::string> first_config_file) :
  ares::config(log, io_context, "zone_server", first_config_file) {
  postgres = load_with_catch_as<postgres_config>("database", json_);
  listen_ipv4 = load_with_catch_as<endpoints_config>("listen_ipv4", json_);
  load_character_server();
  load_network_threads();
  load_obfuscation_key();
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

void ares::zone::config::load_network_threads() {
  auto load_network_threads = [this] () {
    auto j_network_threads = json_.find("network_threads");
    if ((j_network_threads != json_.end() && (j_network_threads->is_number()))) {
      network_threads.emplace(*j_network_threads);
    }
  };
  with_catch("network_threads", load_network_threads);
}

void ares::zone::config::load_obfuscation_key() {
  auto load_obfuscation_key = [this] () {
    auto j_key = json_.find("obfuscation_key");
    if (j_key != json_.end()) {
      if ((j_key->is_array()) && (j_key->size() == 3)) {
        uint32_t k[3];
        for (size_t i = 0; i < 3; ++i) {
          std::stringstream ss;
          ss << std::hex << (*j_key)[i].get<std::string>();
          ss >> k[i]; 
        }

        obfuscation_key.emplace(std::make_tuple(k[0], k[1], k[2]));
        log_->info("New sessions will be initialized with obfuscation key: {:x}, {:x}, {:x}",
                   std::get<0>(*obfuscation_key), std::get<1>(*obfuscation_key), std::get<2>(*obfuscation_key)); 
      } else {
        log_->warn("Packet header obfuscation key should be an array of 3 itegers");
      }
    }
  };
  with_catch("obfuscation_key", load_obfuscation_key);
}
