#pragma once

#include <ares/database>

#include "../config.hpp"
#include "structs.hpp"

namespace ares {
  namespace zone {
    struct database : ares::database<database> {
      database(std::shared_ptr<spdlog::logger> log,
               const config::postgres_config& conf);

    };
  }
}
