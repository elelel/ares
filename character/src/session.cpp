#include "session.hpp"

#include <utility>

#include "state.hpp"
#include "mono/state.hpp"

ares::character::session::session(character::state& server_state,
                                  std::shared_ptr<asio::ip::tcp::socket> socket) :
  ares::network::session<session>(server_state.io_service(), server_state.log(), socket),
  session_state_(std::in_place_type<mono::state>, server_state, *this),
  server_state_(server_state) {
}

void ares::character::session::defuse_asio() {
  close_socket();
  inactivity_timer_.cancel();
  if (is_mono()) return as_mono().defuse_asio();
  if (is_client()) return as_client().defuse_asio();
  if (is_account_server()) return as_account_server().defuse_asio();
  if (is_zone_server()) return as_zone_server().defuse_asio();
}

void ares::character::session::remove_from_server() {
  defuse_asio();
  server_state_.server.remove(shared_from_this());
}

void ares::character::session::on_disconnect() {
  remove_from_server();
}

void ares::character::session::on_open() {
}

void ares::character::session::before_close() {
}

void ares::character::session::on_connection_reset() {
  SPDLOG_TRACE(log_, "character::session:on_connection_reset");
  on_disconnect();
}

void ares::character::session::on_eof() {
  SPDLOG_TRACE(log_, "character::session:on_eof");
  on_disconnect();
}

void ares::character::session::on_socket_error() {
  SPDLOG_TRACE(log_, "character::session:on_socket_error");
  remove_from_server();
}

void ares::character::session::on_operation_aborted() {
  SPDLOG_TRACE(log_, "character::session:on_operation_aborted");
}


auto ares::character::session::variant() -> state_variant& {
  return session_state_;
}

bool ares::character::session::is_mono() const {
  return std::holds_alternative<mono::state>(session_state_);
}

auto ares::character::session::as_mono() -> mono::state& {
  return std::get<mono::state>(session_state_);
}

bool ares::character::session::is_client() const {
  return std::holds_alternative<client::state>(session_state_);
}

auto ares::character::session::as_client() -> client::state& {
  return std::get<client::state>(session_state_);
}

bool ares::character::session::is_zone_server() const {
  return std::holds_alternative<zone_server::state>(session_state_);
}

auto ares::character::session::as_zone_server() -> zone_server::state& {
  return std::get<zone_server::state>(session_state_);
}

bool ares::character::session::is_account_server() const {
  return std::holds_alternative<zone_server::state>(session_state_);
}

auto ares::character::session::as_account_server() -> account_server::state& {
  return std::get<account_server::state>(session_state_);
}

auto ares::character::session::make_send_handler() -> send_handler {
  return send_handler(shared_from_this());
}

auto ares::character::session::make_recv_handler() -> recv_handler {
  return recv_handler(shared_from_this());

}


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

void ares::character::session::inactivity_timer_handler::operator()(const std::error_code& ec) {
  if (ec.value() == 0) session_->on_inactivity_timer();
}
