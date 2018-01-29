#include "state.hpp"

ares::character::state::state() :
  io_service_(std::make_shared<asio::io_service>()),
  log_(spdlog::stdout_color_mt("character")),   // This should default to non-color?
  conf(log_, io_service_, std::optional<std::string>()),
  db(log_, *conf.postgres),
  server(*this) {
}

std::shared_ptr<asio::io_service> ares::character::state::io_service() const {
  return io_service_;
}

std::shared_ptr<spdlog::logger> ares::character::state::log() const {
  return log_;
}
  
