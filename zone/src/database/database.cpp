#include "database.hpp"

ares::zone::database::database(std::shared_ptr<spdlog::logger> log, const config::postgres_config& conf) :
  ares::database<database>(log, conf->dbname, conf->host, conf->port, conf->user, conf->password) {
  SPDLOG_TRACE(log, "ares::zone::database::database");

  if (pqxx_conn_) {
    pqxx_conn_->prepare("whole_map_index", R"(SELECT id, external_id, name FROM map_index )");
  }
}

