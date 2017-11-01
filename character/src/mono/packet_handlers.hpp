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
    namespace mono {
      ARES_DECLARE_PACKET_HANDLER_TEMPLATE();
      
      // Simple packet handlers that do not define their own class structure
      ARES_SIMPLE_PACKET_HANDLER(ATHENA_ZH_LOGIN_REQ);
      
      // Packet handlers that store state/structured
      #include "CH_ENTER.hpp"
    }
  }
}

