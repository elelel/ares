#pragma once

#include "fixed_string.hpp"

namespace ares {
  namespace model {
    struct login_string : fixed_string<24> {
      using fixed_string<24>::fixed_string;
    };
  }
}
