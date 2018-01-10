#pragma once

#include <variant>

#include "mono/state.hpp"
#include "client/state.hpp"
#include "zone_server/state.hpp"
#include "account_server/state.hpp"

namespace ares {
  namespace character {
    using session_state_variant_type = std::variant<mono::state, client::state, zone_server::state, account_server::state>;

  }
}
