#include "state.hpp"

#include <algorithm>

ares::zone::state::state() :
  io_service_(std::make_shared<asio::io_service>()),
  log_(spdlog::stdout_color_mt("zone")),   // This should default to non-color?
  conf(log_, io_service_, std::optional<std::string>()),
  db(log_, *conf.postgres),
  server(*this) {
  std::vector<db::record::map_index> known_maps = db.whole_map_index();
  std::vector<std::string> unknown_maps;
  for (const auto& lhs : conf.maps) {
    if (std::find_if(known_maps.begin(), known_maps.end(), [&lhs] (const db::record::map_index& rhs) {
          return lhs == rhs.name;
        }) == known_maps.end()) {
      unknown_maps.push_back(lhs);
    } else {
      map_names.insert(lhs);
    }
  }
  if (unknown_maps.size() > 0) {
    bool need_comma = false;
    std::string msg;
    for (const auto& m : unknown_maps) {
      if (need_comma) msg += ", ";
      msg += m;
    }
    log_->error("Configured map names that are not in SQL database and therefore will be ignored: {}", msg);
  }
  log_->info("Loaded {} map names", map_names.size());
}

std::shared_ptr<asio::io_service> ares::zone::state::io_service() const {
  return io_service_;
}

std::shared_ptr<spdlog::logger> ares::zone::state::log() const {
  return log_;
}

/* TODO: event loop
std::vector<rxo::observable<ares::zone::world::reducer>> ares::zone::world::reducers{}

  rxcpp::schedulers::run_loop rl;
  auto rl_thread = rxcpp::observe_on_run_loop(rl);
  rxcpp::composite_subscription lifetime;

  while (lifetime.is_subscribed()) {
    while (!rl.empty() && rl.peek().when < rl.now()) {
      rl.dispatch();
    }
  }
*/
