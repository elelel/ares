#pragma once

#include <memory>
#include <optional>
#include <tuple>

#include <spdlog/spdlog.h>

#include <ares/network>
#include <ares/packets>

#include "../predeclare.hpp"

namespace ares {
  namespace zone {
    namespace client {
      ARES_DECLARE_PACKET_HANDLER_TEMPLATE();

      // Simple packet handlers that do not define their own class structure
      ARES_SIMPLE_PACKET_HANDLER(CZ_REQUEST_MOVE);
      
      // Packet handlers that store state/structured

    }
  }
}
