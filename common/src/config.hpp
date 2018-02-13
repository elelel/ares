#pragma once

#include <optional>

#include <asio.hpp>
#include <spdlog/spdlog.h>
#include <nlohmann/json.hpp>

#include <string>
#include <vector>

namespace ares {
  struct config {
    config(std::shared_ptr<spdlog::logger> log,
           std::shared_ptr<asio::io_service> io_service,
           const std::string& service_name,
           const std::optional<std::string> config_fn);

    using endpoint_config = std::optional<asio::ip::tcp::endpoint>;
    using endpoints_config = std::vector<asio::ip::tcp::endpoint>;
    using ipv4_addr_config = std::optional<asio::ip::address>;
    
    // Database configuration
    struct postgres_config_record {
      std::string dbname;
      std::string host;
      uint16_t port{0};
      std::string user;
      std::string password;
    };
    using postgres_config = std::optional<postgres_config_record>;



  protected:
    std::shared_ptr<spdlog::logger> log_;
    std::shared_ptr<asio::io_service> io_service_;
    nlohmann::json json_;

    template <typename Config>
    Config load(const nlohmann::json& j);

    template <typename F>
    void with_catch(const std::string& name, F f) {
      try {
        f();
      } catch (nlohmann::json::parse_error& e) {
        log_->error("config: in json procedure '" + name + "' error " + std::string(e.what()));
        log_->flush();
        throw;
      } catch (nlohmann::json::type_error& e) {
        log_->error("config: in json procedure '" + name + "' error " + std::string(e.what()));
        log_->flush();
        throw;
      } catch (...) {
        log_->error("config: in json procedure '" + name + "' unknown error");
        log_->flush();
        throw;
      }
    }
    
    template <typename Result, typename F>
    Result with_catch_as(const std::string& name, F f) {
      try {
        Result rslt = f();
        return rslt;
      } catch (nlohmann::json::parse_error& e) {
        log_->error("config: in json procedure '" + name + "' error " + std::string(e.what()));
        log_->flush();
        throw;
      } catch (nlohmann::json::type_error& e) {
        log_->error("config: in json procedure '" + name + "' error " + std::string(e.what()));
        log_->flush();
        throw;
      } catch (...) {
        log_->error("config: in json procedure '" + name + "' unknown error");
        log_->flush();
        throw;
      }
    }

    template <typename Result>
    Result load_with_catch_as(const std::string& name, const nlohmann::json& j) {
      return with_catch_as<Result>(name, [this, &j, &name] {
          auto found = j.find(name);
          if (found != j.end()) {
            return with_catch_as<Result>(name + " contents", [this, &found] () {
                return load<Result>(*found);
              });
          } else {
            return Result();
          }
        });
    }

  private:
    void read_from_files(const std::string& service_name,
                         const std::optional<std::string> config_fn);
  };
}
