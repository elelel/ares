#include "config.hpp"

ares::character::config::config(std::shared_ptr<spdlog::logger> log,
                              std::shared_ptr<asio::io_service> io_service,
                              std::optional<std::string> first_config_file) :
  ares::config(log, io_service, "character_server", first_config_file) {
  postgres = load_with_catch_as<postgres_config>("database", json_);
  listen_ipv4 = load_with_catch_as<endpoints_config>("listen_ipv4", json_);
  load_zone_servers();
  load_account_server();
  load_server_name();
  load_priv_msg_server_name();
  load_normal_slots();
  load_premium_slots();
  load_billing_slots();
  load_creatable_slots();
  load_playable_slots();
  load_bank_vault();
  load_max_storage();
  load_grfs();
  validate();
}

void ares::character::config::validate() {
  bool need_comma = false;
  std::string msg;
  if (!server_name) {
    if (need_comma) msg += ", ";
    msg += "server name is missing";
    need_comma = true;
  }
  if (!account_server) {
    if (need_comma) msg += ", ";
    msg += "account server is missing";
    need_comma = true;
  }
  if (zone_servers.size() == 0) {
    if (need_comma) msg += ", ";
    msg += "zone servers are missing";
    need_comma = true;
  }

  std::unordered_set<std::string> seen;
  for (const auto& zs : zone_servers) {
    if (zs.maps.size() == 0) {
      if (need_comma) msg += ", ";
      msg += "zone server '" + zs.login +"' has no maps";
    }
    for (const auto& m : zs.maps) {
      if (seen.find(m) != seen.end()) {
        if (need_comma) msg += ", ";
        msg += "duplicate map " + m;
      }
      seen.insert(m);
    }
  }
  
  // TODO: Check that numbers in slots parameters make sense

  if (msg.size() > 0) {
    msg = "Configuration check failed: " + msg;
    log_->error(msg);
    throw std::runtime_error(msg);
  }
}

void ares::character::config::load_zone_servers() {
  auto load_zone_servers = [this] () {
    auto j_zone_servers = json_.find("zone_servers");
    if ((j_zone_servers != json_.end()) && (j_zone_servers->is_array())) {
      for (const auto& j_zone_server : *j_zone_servers) {
        zone_server_config_record r;
        bool ok{true};
        if (j_zone_server.find("login") != j_zone_server.end())
          r.login = j_zone_server.at("login");
        else {
          ok = false;
          log_->error("Zone server config record doesn't contain 'login' field");
        }
        if (j_zone_server.find("password") != j_zone_server.end())
          r.password = j_zone_server.at("password");
        else {
          ok = false;
          log_->error("Zone server config record doesn't contain 'password' field");
        }
        auto j_maps = j_zone_server.find("maps");
        if (j_maps != j_zone_server.end()) {
          for (const auto& j_map : *j_maps) {
            if (j_map.is_string()) r.maps.insert(j_map.get<std::string>()); else {
              log_->error("Map name in zone server config is not a string");
            }
          }
        } else {
          log_->error("Zone server config record doesn't contain 'maps' field");
        }
        if (ok) zone_servers.push_back(r);
      }
    } else {
      log_->warn("No character servers configured");
    }
  };
  with_catch("zone_servers", load_zone_servers);
}

void ares::character::config::load_account_server() {
  auto load_account_server = [this] () {
    auto j_account_server = json_.find("account_server");
    if ((j_account_server != json_.end()) && (j_account_server->is_object())) {
      account_server_config_record r;
      bool ok{true};
      if (j_account_server->find("connect") != j_account_server->end()) {
        r.connect = load<endpoint_config>(j_account_server->at("connect"));
      } else {
        ok = false;
        log_->error("Account server config record doesn't containt 'connect' field");
      }
      if (j_account_server->find("login") != j_account_server->end())
        r.login = j_account_server->at("login");
      else {
        ok = false;
        log_->error("Account server config record doesn't contain 'login' field");
      }
      if (j_account_server->find("password") != j_account_server->end())
        r.password = j_account_server->at("password");
      else {
        ok = false;
        log_->error("Account server config record doesn't contain 'password' field");
      }
      if (ok) account_server.emplace(r);
    } else {
      log_->warn("No account server configured");
    }
  };
  with_catch("account_server", load_account_server);
}

void ares::character::config::load_priv_msg_server_name() {
   auto load_priv_msg_server_name = [this] () {
    auto j_priv_msg_server_name = json_.find("priv_msg_server_name");
    if ((j_priv_msg_server_name != json_.end()) && (j_priv_msg_server_name->is_string())) {
      priv_msg_server_name.emplace(j_priv_msg_server_name->get<std::string>());
    } else {
      priv_msg_server_name = "Server";
    }
  };
  with_catch("priv_msg_server_name", load_priv_msg_server_name);
}

void ares::character::config::load_server_name() {
  auto load_server_name = [this] () {
    auto j_server_name = json_.find("server_name");
    if ((j_server_name != json_.end()) && (j_server_name->is_string())) {
      server_name.emplace(j_server_name->get<std::string>());
    } else {
      server_name = "Server";
    }
  };
  with_catch("server_name", load_server_name);
}

void ares::character::config::load_normal_slots() {
  auto load_normal_slots = [this] () {
    auto j_normal_slots = json_.find("normal_slots");
    if ((j_normal_slots != json_.end()) && (j_normal_slots->is_number())) {
      normal_slots = *j_normal_slots;
    } else {
      normal_slots = 9;
    }
  };
  with_catch("normal_slots", load_normal_slots);
}

void ares::character::config::load_premium_slots() {
  auto load_premium_slots = [this] () {
    auto j_premium_slots = json_.find("premium_slots");
    if ((j_premium_slots != json_.end()) && (j_premium_slots->is_number())) {
      premium_slots = *j_premium_slots;
    } else {
      premium_slots = 0;
    }
  };
  with_catch("premium_slots", load_premium_slots);
}

void ares::character::config::load_billing_slots() {
  auto load_billing_slots = [this] () {
    auto j_billing_slots = json_.find("billing_slots");
    if ((j_billing_slots != json_.end()) && (j_billing_slots->is_number())) {
      billing_slots = *j_billing_slots;
    } else {
      billing_slots = 0;
    }
  };
  with_catch("billing_slots", load_billing_slots);
}

void ares::character::config::load_creatable_slots() {
  auto load_creatable_slots = [this] () {
    auto j_creatable_slots = json_.find("creatable_slots");
    if ((j_creatable_slots != json_.end()) && (j_creatable_slots->is_number())) {
      creatable_slots = *j_creatable_slots;
    } else {
      creatable_slots = 9;
    }
  };
  with_catch("creatable_slots", load_creatable_slots);
}

void ares::character::config::load_playable_slots() {
  auto load_playable_slots = [this] () {
    auto j_playable_slots = json_.find("playable_slots");
    if ((j_playable_slots != json_.end()) && (j_playable_slots->is_number())) {
      playable_slots = *j_playable_slots;
    } else {
      playable_slots = 9;
    }
  };
  with_catch("playable_slots", load_playable_slots);
}

void ares::character::config::load_bank_vault() {
  auto load_bank_vault = [this] () {
    auto j_bank_vault = json_.find("bank_vault");
    if ((j_bank_vault != json_.end()) && (j_bank_vault->is_number())) {
      bank_vault = *j_bank_vault;
    } else {
      bank_vault = 0;
    }
  };
  with_catch("bank_vault", load_bank_vault);
}

void ares::character::config::load_max_storage() {
  auto load_max_storage = [this] () {
    auto j_max_storage = json_.find("max_storage");
    if ((j_max_storage != json_.end()) && (j_max_storage->is_number())) {
      max_storage = *j_max_storage;
    } else {
      max_storage = 0;
    }
  };
  with_catch("max_storage", load_max_storage);
}

void ares::character::config::load_grfs() {
  auto load_grfs = [this] () {
    auto j_grfs = json_.find("grfs");
    if (j_grfs != json_.end()) {
      if (j_grfs->is_array()) {
        for (const auto& grf : *j_grfs) {
          grfs.push_back(grf);
        }
      } else {
        log_->warn("List of grfs files in config should be an array");
      }
    }
  };
  with_catch("load_grfs", load_grfs);
}

