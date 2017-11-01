#pragma once

#include <variant>

#include "mono/state.hpp"
#include "client/state.hpp"
#include "character_server/state.hpp"

namespace ares {
  namespace zone {
    using state_variant_type = std::variant<mono::state, client::state, character_server::state>;
  }
}
