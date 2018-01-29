#include "config.hpp"

#include <fstream>
#include <system_error>

ares::config::config(std::shared_ptr<spdlog::logger> log,
                     std::shared_ptr<asio::io_service> io_service,
                     const std::string& service_name,
                     const std::optional<std::string> config_fn) :
  log_(log),
  io_service_(io_service) {
  read_from_files(service_name, config_fn);
}

void ares::config::read_from_files(const std::string& service_name,
                                   const std::optional<std::string> config_fn) {
  // The vector contains all possible config fully-quallyfied filenames.
  // The lower the index of an element, the higher priority of this config file.
  std::vector<std::string> conf_filenames;
  if (config_fn) {
    // If config filename is specified explicitly, use it as the one with the highest
    // priority
    conf_filenames.push_back(*config_fn);
  } else {
    // Add config file from current user's home directory's 'etc' subdir
    // Using HOME system env variable is a portable way of getting user homedir on Unix and Windows
    char const* home_ptr = getenv("HOME");
    if (home_ptr) conf_filenames.push_back(std::string(home_ptr) + "/etc/ares/" + service_name + ".json");
    conf_filenames.push_back("/etc/ares/" + service_name + ".json");
  }

  std::ifstream conf_file;
  // Iterate the filename vector until a file is opened successfuly
  for (auto fn = conf_filenames.begin(); (fn != conf_filenames.end()) && !conf_file.is_open(); ++fn) {
    SPDLOG_TRACE(log_, "config: trying to open file {}", *fn);
    conf_file.open(*fn);
  }
  if (conf_file.is_open()) {
    if (conf_file.good()) {
      conf_file >> json_;
    } else {
      log_->error("config: file stream error after open");
      log_->flush();
      throw std::runtime_error("config: file stream error after open");
    }
  } else {
    std::string msg;
    bool need_comma{false};
    for (const auto& fn : conf_filenames) {
      if (need_comma) msg += ", ";
      msg += fn;
      need_comma = true;
    }
    log_->error("config: could not open configuration file, tried the following filenames - {}", msg);
    log_->flush();
    throw std::runtime_error("config: could not open configuration file");
  }
}

template <>
auto ares::config::load<ares::config::postgres_config>(const nlohmann::json& j) -> postgres_config {
  postgres_config rslt;
  postgres_config_record r;
  if (j.find("host") != j.end())
    r.host = j.at("host");
  if (j.find("port") != j.end())
    r.port = j.at("port");
  if (j.find("user") != j.end())
    r.user = j.at("user");
  if (j.find("password") != j.end())
    r.password = j.at("password");
      
  if (j.find("dbname") != j.end()) {
    r.dbname = j.at("dbname");
    rslt.emplace(r);
  } else {
    log_->error("config: database name ('dbname') has to be specified in Postgres connection configuration");
  }
  return rslt;
}

template <>
auto ares::config::load<ares::config::ipv4_addr_config>(const nlohmann::json& j) -> ipv4_addr_config {
  using namespace asio;
  ipv4_addr_config rslt;
  if (j.is_string()) {
    auto host = j.get<std::string>();
    std::error_code ec;
    auto addr = ip::address::from_string(host, ec);
    if (ec.value() == 0) {
      rslt.emplace(addr);
    } else {
      // It's probably a name
      ip::tcp::resolver::query q(host, 0, ip::tcp::resolver::query::numeric_service);
      ip::tcp::resolver r(*io_service_);
      auto it = r.resolve(q, ec);
      if ((ec.value() == 0) && (it != ip::tcp::resolver::iterator())) {
        rslt.emplace(it->endpoint().address());
      } else {
        log_->error("config: ipv4_addr host '{}' is neither ip address, nor a resolvable hostname", host);
      }
    }
  } else {
    log_->error("config: ipv4_addr expected a string json type");
  }
  return rslt;
}

template <>
auto ares::config::load<ares::config::endpoint_config>(const nlohmann::json& j) -> endpoint_config {
  using namespace asio;
  endpoint_config rslt;
  if (j.is_object()) {
    if ((j.find("host") != j.end()) && (j.find("port") != j.end())) {
      std::string host = j.at("host");
      uint16_t port = j.at("port");
      std::error_code ec;
      auto addr = ip::address::from_string(host, ec);
      if (ec.value() == 0) {
        rslt.emplace(ip::tcp::endpoint(addr, port));
      } else {
        // It's porbably a name
        ip::tcp::resolver::query q(host, std::to_string(port), ip::tcp::resolver::query::numeric_service);
        ip::tcp::resolver r(*io_service_);
        auto it = r.resolve(q, ec);
        if ((ec.value() == 0) && (it != ip::tcp::resolver::iterator())) {
          rslt.emplace(*it);
        } else {
          log_->error("config: endpoint host '{}' is neither ip address, nor a resolvable hostname", host);
        }
      }
    } else {
      log_->error("config: endpoint record should contain 'host' and 'port' fields");
    }
  } else {
    log_->error("config: endpoint should be of object json type");
  }
  return rslt;
}

template <>
auto ares::config::load<ares::config::endpoints_config>(const nlohmann::json& j) -> endpoints_config {
  using namespace asio;
  endpoints_config rslt;
  if (j.is_array()) {
    for (const auto& r : j) {
      endpoint_config ep = load<endpoint_config>(r);
      if (ep) rslt.push_back(*ep);
    }
  } else {
    log_->error("config: endpoints should be of array json type");
  }
  return rslt;
}
