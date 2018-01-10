#pragma once

#include <functional>
#include <memory>
#include <vector>

#include <rxcpp/rx.hpp>

#include "event/pc_move.hpp"

namespace ares {
  namespace zone {
    namespace world {
      struct state {
        /*! Coordination to work on separate threads */
        decltype(rxcpp::observe_on_event_loop()) pool_thread = rxcpp::observe_on_event_loop();
        /*! Stream of PC move commands from client */
        rxcpp::rxsub::subject<event::pc_move_command> pc_move_commands;
      };
    }
  }
}
