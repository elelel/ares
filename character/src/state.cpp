#include "state.hpp"

ares::character::state::state() :
  io_service_(std::make_shared<asio::io_service>()),
  log_(spdlog::stdout_color_mt("character")),   // This should default to non-color?
  conf(log_, io_service_, std::optional<std::string>()),
  db(log_, *conf.postgres),
  server(*this) {
  log_->set_level(spdlog::level::trace);
  std::vector<db::record::map_index> known_maps = db.whole_map_index();
  std::vector<std::string> unknown_maps;
  for (const auto& zs : conf.zone_servers) {
    for (const auto& lhs : zs.maps) {
      if (std::find_if(known_maps.begin(), known_maps.end(), [&lhs] (const db::record::map_index& rhs) {
            return lhs == rhs.name;
          }) == known_maps.end()) {
        unknown_maps.push_back(lhs);
      }
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
    throw std::runtime_error("Configured map names that are not in SQL database and therefore will be ignored: " + msg);
  }
}

std::shared_ptr<asio::io_service> ares::character::state::io_service() const {
  return io_service_;
}

std::shared_ptr<spdlog::logger> ares::character::state::log() const {
  return log_;
}
  
