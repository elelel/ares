#pragma once

#include <chrono>
#include <string>
#include <vector>

namespace ares {
  namespace database {
    namespace record {
      struct map_index {
        uint32_t id;
        uint32_t external_id;
        std::string name;
      };
    }
  }
}
