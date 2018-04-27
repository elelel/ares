#include "map.hpp"

#include <ares/common/memory>

#include "../pc/pc.hpp"
#include "event_visitor.hpp"

ares::zone::map::map(std::shared_ptr<spdlog::logger> log,
                     std::shared_ptr<asio::io_context> io_context,
                     const uint32_t id, const model::map_info& info) :
  id_(id),
  info_(info),
  io_context_(io_context),
  log_(log) {
  }

bool ares::zone::map::is_walkable(const uint16_t x, const uint16_t y) const {
  return (x < info_.x_size()) && (y < info_.y_size()) &&
    info_.static_flags(x, y).data() & model::map_cell_flags::walkable;
}

std::mutex& ares::zone::map::mutex() {
  return mutex_;
}

bool ares::zone::map::add_pc(std::shared_ptr<zone::pc> pc) {
  // TODO: Replace existing pc instead?
  auto found = pcs_.find(pc->info.character_id);
  if (found == pcs_.end()) {
    pcs_.insert(std::pair<const model::character_id, std::weak_ptr<zone::pc>>{pc->info.character_id, pc});
    start_tick_timer();
    return true;
  };
  return false;
}

void ares::zone::map::remove_pc(const model::character_id cid) {
  pcs_.erase(cid);
  if (pcs_.size() == 0) stop_tick_timer();
}

void ares::zone::map::remove_pc(std::shared_ptr<zone::pc> pc) {
  auto found = pcs_.find(pc->info.character_id);
  if (found != pcs_.end()) {
    pcs_.erase(found);
  }
  if (pcs_.size() == 0) stop_tick_timer();
}

void ares::zone::map::on_tick() {
  // Should be called under map lock
  map_event e;
  pmr::map<std::chrono::steady_clock::time_point, map_event> sorted_events(&memory::thread_local_pool());
  while (event_queue.try_dequeue(e)) {
    const std::chrono::steady_clock::time_point& ts = std::visit(map_event_timestamp_visitor(), e);
    sorted_events.insert(std::pair<const std::chrono::steady_clock::time_point, map_event>({ts, e}));
  }

  auto tick_time = std::chrono::steady_clock::now();
  // Process events in chronological order until the map is empty.
  // Visitor may add more events to the map if the tick rate was low and
  // it determined that events derived from an event being processed should have happened already.
  while (sorted_events.size() > 0) {
    std::visit(map_event_visitor(*this, tick_time, sorted_events), sorted_events.begin()->second);
    sorted_events.erase(sorted_events.begin());
  }
}

void ares::zone::map::start_tick_timer() {
  if (!tick_timer) {
    tick_timer = std::make_shared<asio::steady_timer>(*io_context_);
    tick_timer->expires_at(std::chrono::steady_clock::now() + map::tick_granularity);
    tick_timer->async_wait(map_tick_handler(shared_from_this()));
  }
}

void ares::zone::map::stop_tick_timer() {
  if (tick_timer) {
    tick_timer->cancel();
  }
}

void ares::zone::map::defuse_asio() {
  stop_tick_timer();
}

auto ares::zone::map::log() const -> std::shared_ptr<spdlog::logger> {
  return log_;
}

ares::zone::map_tick_handler::map_tick_handler(std::weak_ptr<zone::map> map) :
  map_(map) {
}

ares::zone::map_tick_handler::map_tick_handler(const map_tick_handler& other) :
  map_(other.map_) {
}

void ares::zone::map_tick_handler::operator()(const std::error_code& ec) {
  auto m = map_.lock();
  if (m) {
    if (ec.value() == 0) {
      std::lock_guard<std::mutex> lock(m->mutex());
      m->on_tick();
      m->tick_timer->expires_at(std::chrono::steady_clock::now() + map::tick_granularity);
      m->tick_timer->async_wait(map_tick_handler(m));
    } else {
      std::lock_guard<std::mutex> lock(m->mutex());
      m->tick_timer = nullptr;
    }
  }
}
