#pragma once

#include <memory>
#include <optional>
#include <tuple>

#include <spdlog/spdlog.h>

#include <ares/network>
#include <ares/packets>

#include "../predeclare.hpp"

namespace ares {
  namespace character {
    namespace zone_server {
      ARES_DECLARE_PACKET_HANDLER_TEMPLATE();

      // Simple packet handlers that do not define their own class structure
      ARES_SIMPLE_PACKET_HANDLER(ATHENA_ZH_MAP_NAMES);
      ARES_SIMPLE_PACKET_HANDLER(ATHENA_ZH_ONLINE_USERS);
      ARES_SIMPLE_PACKET_HANDLER(ATHENA_ZH_USER_COUNT);
      ARES_SIMPLE_PACKET_HANDLER(ATHENA_ZH_GAME_RATES);
      ARES_SIMPLE_PACKET_HANDLER(ATHENA_ZH_PING_REQ);
      
      // Packet handlers that store state/structured
    }
  }
}
