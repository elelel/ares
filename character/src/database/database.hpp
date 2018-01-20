#pragma once

#include <ares/database>

#include "../config.hpp"
#include "structs.hpp"

namespace ares {
  namespace character {
    struct database : ares::database<database> {
      database(std::shared_ptr<spdlog::logger> log,
               const config::postgres_config& conf);

      /* Load account data record for a given aid */
      std::optional<db::record::acc_data> acc_data_for_aid(const uint32_t aid);
      /* Load vector of char data records for a given aid */
      std::vector<db::record::char_data> char_data_for_aid(const uint32_t aid, const size_t max_chars);
    };
  }
}
