#pragma once

#include "predeclare.hpp"

#include "state_struct.hpp"

namespace ares {
  namespace zone {
    struct state_shared {
      std::shared_ptr<state_struct> data = std::make_shared<state_struct>();
    };
  }
}
