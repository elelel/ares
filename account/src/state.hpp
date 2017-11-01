#pragma once

#include <variant>

#include "mono/state.hpp"
#include "client/state.hpp"
#include "char_server/state.hpp"

namespace ares {
  namespace account {
    using state_variant_type = std::variant<mono::state, client::state, char_server::state>;
  }
}
