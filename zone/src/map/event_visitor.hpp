#pragma once

#include <ares/common/memory>

#include "events.hpp"

namespace ares {
  namespace zone {

    struct map;

    struct map_event_timestamp_visitor {
      template <typename T>
      inline const std::chrono::steady_clock::time_point& operator()(const T& e) {
        return e->timestamp;
      }
    };
    
    struct map_event_visitor {
      map_event_visitor(zone::map& map,
                        const std::chrono::steady_clock::time_point& tick_time,
                        pmr::map<std::chrono::steady_clock::time_point, map_event>& sorted_events);

      void operator()(std::shared_ptr<event::pc_walking> e);

    private:
      zone::map& map_;
      const std::chrono::steady_clock::time_point& tick_time_;
      pmr::map<std::chrono::steady_clock::time_point, map_event>& sorted_events_;
    };
  }
}
