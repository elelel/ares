#include "state.hpp"

#include "../server.hpp"
#include "../pc/pc.hpp"
#include "../map/astar.hpp"

void ares::zone::client::packet_handler<ares::packet::current<ares::packet::CZ_REQUEST_MOVE>>::operator()() {
  SPDLOG_TRACE(log(), "handle_packet CZ_REQUEST_MOVE: begin");

  SPDLOG_TRACE(log(), "CZ_REQUEST_MOVE: to x = {}, to y = {}, dir = {}", p_->coords().x(), p_->coords().y(), p_->coords().dir());
  if (auto pc = state_.pc.lock()) {
    if (auto map = pc->map.lock()) {
      auto start = a_star::space_node(*pc->info.location_last.coords.x(), *pc->info.location_last.coords.y());
      auto goal = a_star::space_node(p_->coords().x(), p_->coords().y());
      auto search = a_star::search_state(*map, start, goal);
      auto path = search.run();
      if (path.size() > 0) {
        auto e = std::make_shared<event::pc_walking>();
        e->pc = state_.pc;
        e->path = path;
        e->path_i = 0;
        map->event_queue.enqueue(e);
      }
    } else {
      SPDLOG_TRACE(log(), "Received CZ_REQUEST_MOVE from pc on a non-existent map");
    }
  } else {
    SPDLOG_TRACE(log(), "Received CZ_REQUEST_MOVE from non-existent pc");
  }
  SPDLOG_TRACE(log(), "handle_packet CZ_REQUEST_MOVE: end");
}
