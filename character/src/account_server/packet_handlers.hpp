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
    namespace account_server {
      ARES_DECLARE_PACKET_HANDLER_TEMPLATE();

      // Simple packet handlers that do not define their own class structure
      ARES_SIMPLE_PACKET_HANDLER(ATHENA_AH_LOGIN_RESULT);
      ARES_SIMPLE_PACKET_HANDLER(ATHENA_AH_PING_ACK);
      
      // Packet handlers that store state/structured
    }
  }
}

