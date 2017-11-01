#pragma once

#include <ares/network>

#include "../predeclare.hpp"

namespace ares {
  namespace zone {
    namespace character_server {
      namespace timer {
#define ARES_TIMER(NAME)                                                \
        struct NAME : ares::network::timer<NAME,                        \
                                           std::chrono::seconds,        \
                                           session>  {                  \
          using ares::network::timer<NAME,                              \
                                     std::chrono::seconds,              \
                                     session>::timer;                   \
          static const char* name() { return ""#NAME""; };              \
          void on_timer();                                              \
        };                                                              \
      

        ARES_TIMER(reconnect);
        ARES_TIMER(ping_request);
        ARES_TIMER(ping_timeout);

#undef ARES_TIMER_HANDLER      
      }
    }
  }
}

