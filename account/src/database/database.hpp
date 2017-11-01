#pragma once

#include <ares/database>

#include "../config.hpp"
#include "structs.hpp"

namespace ares {
  namespace account {
    struct database : ares::database<database> {
      database(std::shared_ptr<spdlog::logger> log,
               const config::postgres_config& conf);

      bool user_exists(const std::string& login);
      void create_user(const std::string& login, const std::string& password);
      bool password_matches(const std::string& login, const std::string& password);
      std::optional<db::record::user_data> user_data_for_login(const std::string& login);
      std::optional<db::record::user_data> user_data_for_aid(const uint32_t aid);
    };
  }
}
