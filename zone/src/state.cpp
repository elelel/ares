#include "state.hpp"

ares::zone::state::state() :
  io_service_(std::make_shared<boost::asio::io_service>()),
  log_(spdlog::stdout_color_mt("zone")),   // This should default to non-color?
  conf(log_, io_service_, std::optional<std::string>()),
  db(log_, *conf.postgres),
  server(*this) {
}

std::shared_ptr<boost::asio::io_service> ares::zone::state::io_service() const {
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
