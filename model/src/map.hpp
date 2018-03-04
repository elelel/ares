#pragma once

#include "pc.hpp"

namespace ares {
  namespace model {
    struct map_info {
      uint16_t x_size;
      uint16_t y_size;
      std::vector<uint8_t> cell_flags;
    };
  }
}
