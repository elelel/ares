#pragma once

#include "pc.hpp"

namespace ares {
  namespace model {
    struct map {
      std::map<uint32_t, model::pc_info> pcs;
    };
  }
}
