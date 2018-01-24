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
    namespace client {
      ARES_DECLARE_PACKET_HANDLER_TEMPLATE();

      // Simple packet handlers that do not define their own class structure
      ARES_SIMPLE_PACKET_HANDLER(PING);
      ARES_SIMPLE_PACKET_HANDLER(CH_MAKE_CHAR);
      ARES_SIMPLE_PACKET_HANDLER(CH_SELECT_CHAR);
      ARES_SIMPLE_PACKET_HANDLER(CH_CHAR_PAGE_REQ);
      
      // Packet handlers that store state/structured

    }
  }
}
