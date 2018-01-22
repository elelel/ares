#include "database.hpp"

ares::character::database::database(std::shared_ptr<spdlog::logger> log, const config::postgres_config& conf) :
  ares::database<database>(log, conf->dbname, conf->host, conf->port, conf->user, conf->password) {
  SPDLOG_TRACE(log, "ares::character::database::database");

  if (pqxx_conn_) {
    // Prepare statements
    pqxx_conn_->prepare("account_create", R"(
WITH s AS (
 INSERT INTO "account_slots" ("aid", "normal_slots", "premium_slots", "billing_slots", "creatable_slots", "playable_slots") VALUES ($1, $2, $3, $4, $5, $6)) 
INSERT INTO "account_storage" ("aid", "bank_vault", "max_storage") VALUES ($1, $7, $8);

)");

    pqxx_conn_->prepare("account_slots_for_aid", R"(
SELECT "normal_slots", "premium_slots", "billing_slots", "creatable_slots", "playable_slots"
FROM "account_slots" WHERE ("aid" = $1) 
)");

    pqxx_conn_->prepare("account_storage_for_aid", R"(
SELECT "bank_vault", "max_storage" FROM "account_storage" WHERE ("aid" = $1)
)");

    pqxx_conn_->prepare("character_info_for_aid", R"(
SELECT "id", "slot", "name", "sex", "job", "base_level", "job_level", "base_exp", "job_exp", "zeny",
  "str", "agi", "vit", "int", "dex", "luk", "max_hp", "hp", "max_sp", "sp", "job_point", "skill_point", "effect_state", "virtue", "honor",
  "head", "body", "weapon", "robe", "shield", "head_top", "head_mid", "head_bottom", "head_palette", "body_palette",
  "map_name", "map_x", "map_y", "delete_date", "rename"
FROM "characters", "char_appearance", "char_stats", "char_location"
WHERE ("aid" = $1) AND ("slot" < $2) LIMIT $2
)");
  
    SPDLOG_TRACE(log, "ares::character::database::database done preparing statements");
  }
}

