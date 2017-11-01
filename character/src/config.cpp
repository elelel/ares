#include "config.hpp"

ares::character::config::config(std::shared_ptr<spdlog::logger> log,
                              std::shared_ptr<boost::asio::io_service> io_service,
                              std::optional<std::string> first_config_file) :
  ares::config(log, io_service, "character_server", first_config_file) {
  postgres = load_with_catch_as<postgres_config>("database", json_);
  listen_ipv4 = load_with_catch_as<endpoints_config>("listen_ipv4", json_);
  load_zone_servers();
  load_account_server();
  load_server_name();
  load_priv_msg_server_name();
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
          log_->error("Char server config record doesn't contain 'login' field");
        }
        if (j_zone_server.find("password") != j_zone_server.end())
          r.password = j_zone_server.at("password");
        else {
          ok = false;
          log_->error("Char server config record doesn't contain 'password' field");
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
      priv_msg_server_name.emplace(*j_priv_msg_server_name);
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
      server_name.emplace(*j_server_name);
    } else {
      server_name = "Server";
    }
  };
  with_catch("server_name", load_server_name);
}
