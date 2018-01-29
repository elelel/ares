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

    pqxx_conn_->prepare("num_chars_for_aid", R"(
SELECT count(*) AS cnt FROM "characters" WHERE ("aid" = $1) AND ("slot" < $2)
)");

    pqxx_conn_->prepare("character_info_for_aid", R"(
SELECT "id", "slot", "name", "sex", "job", "base_level", "job_level", "base_exp", "job_exp", "zeny",
  "str", "agi", "vit", "int", "dex", "luk",
  "max_hp", "hp", "max_sp", "sp", "job_point", "skill_point", "effect_state", "body_state", "health_state", "virtue", "honor",
  "head", "body", "weapon", "robe", "shield", "head_top", "head_mid", "head_bottom", "head_palette", "body_palette",
  "map_name", "map_x", "map_y", "delete_date", "rename"
FROM "characters"
JOIN "char_appearance" ON ("char_appearance"."cid" = "id")
JOIN "char_stats" ON ("char_stats"."cid" = "id")
JOIN "char_location" ON ("char_location"."cid" = "id")
WHERE ("aid" = $1) AND ("slot" < $2)
)");
    
    pqxx_conn_->prepare("character_info", R"(
SELECT "id", "slot", "name", "sex", "job", "base_level", "job_level", "base_exp", "job_exp", "zeny",
  "str", "agi", "vit", "int", "dex", "luk",
  "max_hp", "hp", "max_sp", "sp", "job_point", "skill_point", "effect_state", "body_state", "health_state", "virtue", "honor",
  "head", "body", "weapon", "robe", "shield", "head_top", "head_mid", "head_bottom", "head_palette", "body_palette",
  "map_name", "map_x", "map_y", "delete_date", "rename"
FROM "characters"
JOIN "char_appearance" ON ("char_appearance"."cid" = "id")
JOIN "char_stats" ON ("char_stats"."cid" = "id")
JOIN "char_location" ON ("char_location"."cid" = "id")
WHERE ("id" = $1)
)");
    
    pqxx_conn_->prepare("character_info_for_slot", R"(
SELECT "id", "slot", "name", "sex", "job", "base_level", "job_level", "base_exp", "job_exp", "zeny",
  "str", "agi", "vit", "int", "dex", "luk",
  "max_hp", "hp", "max_sp", "sp", "job_point", "skill_point", "effect_state", "body_state", "health_state", "virtue", "honor",
  "head", "body", "weapon", "robe", "shield", "head_top", "head_mid", "head_bottom", "head_palette", "body_palette",
  "map_name", "map_x", "map_y", "delete_date", "rename"
FROM "characters"
JOIN "char_appearance" ON ("char_appearance"."cid" = "id")
JOIN "char_stats" ON ("char_stats"."cid" = "id")
JOIN "char_location" ON ("char_location"."cid" = "id")
WHERE ("aid" = $1) AND ("slot" = $2)
)");
    
    pqxx_conn_->prepare("make_char_create_cid", R"(
INSERT INTO "characters" ("aid", "slot", "name", "sex", "job", "rename") VALUES ($1, $2, $3, $4, $5, 0)
)");

    pqxx_conn_->prepare("cid_by_name", R"(
SELECT "id" FROM "characters" WHERE "name" = $1
)");

    pqxx_conn_->prepare("make_char_create_stats", R"(
INSERT INTO "char_stats" ("cid", "base_level", "job_level", "base_exp", "job_exp", "zeny",
 "str", "agi", "vit", "int", "dex", "luk", "max_hp", "hp", "max_sp", "sp",
 "job_point", "skill_point", "effect_state", "body_state", "health_state", "virtue", "honor")
VALUES ($1, 1, 1, 0, 0, $2, 1, 1, 1, 1, 1, 1, $3, $3, $4, $4, 48, 0, 0, 0, 0, 0, 0)
)");

    pqxx_conn_->prepare("make_char_create_appearance", R"(
INSERT INTO "char_appearance" ("cid", "head", "body", "weapon", "shield", "robe", "head_top", "head_mid", "head_bottom", "head_palette", "body_palette")
VALUES ($1, $2, 0, 0, 0, 0, 0, 0, 0, $3, 0)
)");

    pqxx_conn_->prepare("make_char_create_location", R"(
INSERT INTO "char_location" ("cid", "map_name", "map_x", "map_y")
VALUES ($1, $2, $3, $4)
)");
    SPDLOG_TRACE(log, "ares::character::database::database done preparing statements");
  }
}

