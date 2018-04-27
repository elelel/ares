#pragma once

#include <chrono>
#include <memory>
#include <variant>
#include <vector>

#include <ares/model>

namespace ares {
  namespace zone {
    struct pc;
    
    namespace event {
      struct event_base {
        std::chrono::steady_clock::time_point timestamp = std::chrono::steady_clock::now();
      };
      
      struct pc_walking : event_base {
        std::weak_ptr<zone::pc> pc;
        std::vector<model::packed_coordinates> path;
        size_t path_i;
      };
    }

    using map_event =
      std::variant<std::shared_ptr<event::pc_walking>>;

  }
}
