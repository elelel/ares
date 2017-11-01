#include "session.hpp"

void ares::character::session::reset_inactivity_timer() {
  inactivity_timer_.cancel();
  inactivity_timer_.expires_from_now(inactivity_timeout_);
  auto handler = inactivity_timer_handler(shared_from_this());
  inactivity_timer_.async_wait(handler);
}

void ares::character::session::on_inactivity_timer() {
  SPDLOG_TRACE(log_, "character::session::on_inactivity_timer");
  remove_from_server();
}

void ares::character::session::inactivity_timer_handler::operator()(const boost::system::error_code& ec) {
  if (ec.value() == 0) session_->on_inactivity_timer();
}
