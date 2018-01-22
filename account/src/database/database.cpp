#include "database.hpp"

ares::account::database::database(std::shared_ptr<spdlog::logger> log, const config::postgres_config& conf) :
  ares::database<database>(log, conf->dbname, conf->host, conf->port, conf->user, conf->password) {
  SPDLOG_TRACE(log, "ares::account::database::database");

  if (pqxx_conn_) {
    // Prepare queries
    pqxx_conn_->prepare("create_user", R"(
INSERT INTO "users" (login, password) VALUES ($1, crypt($2, gen_salt('bf')));
)");

    pqxx_conn_->prepare("password_matches", R"(
SELECT "id" FROM "users"
    WHERE ("login" = $1) AND (password = crypt($2, password)) LIMIT 1;
)");

    pqxx_conn_->prepare("user_data_for_login", R"(
SELECT "id", "login", "email", "level", "sex", "expiration_time", "birthdate", "pin"  FROM "users"
    WHERE ("login" = $1) LIMIT 1;
)");
    pqxx_conn_->prepare("user_data_for_aid", R"(
SELECT "id", "login", "level", "email", "sex", "expiration_time", "birthdate", "pin"  FROM "users"
  WHERE ("id" = $1) LIMIT 1;
)");

    pqxx_conn_->prepare("user_exists", R"(
SELECT "id" FROM "users" WHERE (login = $1) LIMIT 1;
)");
  SPDLOG_TRACE(log, "ares::account::database::database done preparing statements");
  }            
}

