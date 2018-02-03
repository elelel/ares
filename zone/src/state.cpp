#include "state.hpp"

#include <algorithm>

ares::zone::state::state() :
  io_service_(std::make_shared<asio::io_service>()),
  log_(spdlog::stdout_color_mt("zone")),   // This should default to non-color?
  conf(log_, io_service_, std::optional<std::string>()),
  db(log_, *conf.postgres),
  server(*this) {
  log_->set_level(spdlog::level::trace);
}

std::shared_ptr<asio::io_service> ares::zone::state::io_service() const {
  return io_service_;
}

std::shared_ptr<spdlog::logger> ares::zone::state::log() const {
  return log_;
}

template <>
void ares::zone::state::post_event(const event::cz_enter& e) {
  rxcpp::rxs::just(e).subscribe(cz_enter_stream.get_subscriber());
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
