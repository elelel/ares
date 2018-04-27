#include "event_visitor.hpp"

#include "map.hpp"
#include "../pc/pc.hpp"
#include "../server.hpp"

void ares::zone::map_event_visitor::operator()(std::shared_ptr<event::pc_walking> e) {
  SPDLOG_TRACE(map_.log(), "pc_walking starting");
  auto pc = e->pc.lock();
  if (pc) {
    auto session = pc->session().lock();
    if (session) {
      auto elapsed = tick_time_ - e->timestamp;
      if (pc->speed() >= elapsed) {
        if ((e->path_i == e->path.size() -1) ||
            (map_.is_walkable(e->path[e->path_i + 1].x(), e->path[e->path_i + 1].y()))) {
          // TODO: Move unit
          // TODO: Send move ack
          e->timestamp += pc->speed();
          ++e->path_i;
          // Put it again on sorted events map to check if we need process next step
          sorted_events_.insert(std::pair<const std::chrono::steady_clock::time_point, map_event>({e->timestamp, map_event(e)}));
          SPDLOG_TRACE(map_.log(), "Sending ZC_NOTIFY_PLAYERMOVE, step {}", e->path_i -1);
          session->emplace_and_send<packet::current<packet::ZC_NOTIFY_PLAYERMOVE>>(e->path[e->path_i - 1].x(), e->path[e->path_i - 1].y(),
                                                                                   e->path[e->path_i].x(), e->path[e->path_i].y());
        } else {
          // No path or the path became unwalkable, just stop walking
          SPDLOG_TRACE(map_.log(), "pc_walking, can't walk further, stopping");
        }
      } else {
        // Reschedule the event for later map tick
        SPDLOG_TRACE(map_.log(), "pc_walking, not elapsed.  speed {}, elapsed {}, tick_time {}, ts {} ",
                     pc->speed().count(), elapsed.count(),
                     tick_time_.time_since_epoch().count(),
                     e->timestamp.time_since_epoch().count());
        map_.event_queue.enqueue(map_event(e));
      }
    }
  }
}
