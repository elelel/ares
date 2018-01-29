#include "session.hpp"

#include <utility>

#include "state.hpp"
#include "mono/state.hpp"

ares::zone::session::session(zone::state& zone_state,
                             std::shared_ptr<asio::ip::tcp::socket> socket) :
  ares::network::session<session>(zone_state.io_service(), zone_state.log(), socket),
  session_state_(std::in_place_type<mono::state>, zone_state, *this),
  server_state_(zone_state) {
}

void ares::zone::session::defuse_asio() {
  close_socket();
  inactivity_timer_.cancel(); 
  if (is_client()) return as_client().defuse_asio();
  if (is_character_server()) return as_character_server().defuse_asio();
  if (is_mono()) return as_mono().defuse_asio();
}

void ares::zone::session::remove_from_server() {
  defuse_asio();
  server_state_.server.remove(shared_from_this());
}

void ares::zone::session::on_disconnect() {
  remove_from_server();
}

void ares::zone::session::on_open() {
}

void ares::zone::session::before_close() {
}

void ares::zone::session::on_connection_reset() {
  SPDLOG_TRACE(log_, "zone::session:on_connection_reset");
  on_disconnect();
}

void ares::zone::session::on_eof() {
  SPDLOG_TRACE(log_, "zone::session:on_eof");
  on_disconnect();
}

void ares::zone::session::on_socket_error() {
  SPDLOG_TRACE(log_, "zone::session:on_socket_error");
  remove_from_server();
}

void ares::zone::session::on_operation_aborted() {
  SPDLOG_TRACE(log_, "zone::session:on_operation_aborted");
}

auto ares::zone::session::variant() -> state_variant& {
  return session_state_;
}

bool ares::zone::session::is_mono() const {
  return std::holds_alternative<mono::state>(session_state_);
}

auto ares::zone::session::as_mono() -> mono::state& {
  return std::get<mono::state>(session_state_);
}

bool ares::zone::session::is_client() const {
  return std::holds_alternative<client::state>(session_state_);
}

auto ares::zone::session::as_client() -> client::state& {
  return std::get<client::state>(session_state_);
}

bool ares::zone::session::is_character_server() const {
  return std::holds_alternative<character_server::state>(session_state_);
}

auto ares::zone::session::as_character_server() -> character_server::state& {
  return std::get<character_server::state>(session_state_);
}

auto ares::zone::session::make_send_handler() -> send_handler {
  return send_handler(shared_from_this());
}

auto ares::zone::session::make_recv_handler() -> recv_handler {
  return recv_handler(shared_from_this());

}

void ares::zone::session::reset_inactivity_timer() {
  inactivity_timer_.cancel();
  inactivity_timer_.expires_from_now(inactivity_timeout_);
  auto handler = inactivity_timer_handler(shared_from_this());
  inactivity_timer_.async_wait(handler);
}

void ares::zone::session::on_inactivity_timer() {
  SPDLOG_TRACE(server_state_.log(), "zone::session::on_inactivity_timer");
  remove_from_server();
}

void ares::zone::session::inactivity_timer_handler::operator()(const std::error_code& ec) {
  if (ec.value() == 0) session_->on_inactivity_timer();
}
