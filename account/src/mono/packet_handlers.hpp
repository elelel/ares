#pragma once

#include <memory>
#include <optional>
#include <tuple>

#include <spdlog/spdlog.h>

#include <ares/network>
#include <ares/packets>

#include "../predeclare.hpp"

namespace ares {
  namespace account {
    namespace mono {
      ARES_DECLARE_PACKET_HANDLER_TEMPLATE();

      // Simple packet handlers that do not define their own class structure
      ARES_SIMPLE_PACKET_HANDLER(ATHENA_HA_LOGIN_REQ);
      ARES_SIMPLE_PACKET_HANDLER(ATHENA_HA_PING_REQ);
      ARES_SIMPLE_PACKET_HANDLER(CA_EXE_HASHCHECK);
      
      // Packet handlers that store state/structured
      #include "CA_SSO_LOGIN_REQ.hpp"
    }
  }
}
