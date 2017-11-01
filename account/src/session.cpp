#include "session.hpp"

#include "server.hpp"

#include <utility>

#include "mono/state.hpp"

ares::account::session::session(server& serv,
                                std::shared_ptr<boost::asio::ip::tcp::socket> socket) :
  ares::network::session<session>(serv.io_service(), serv.log(), socket),
  server_(serv),
  state_(std::in_place_type<mono::state>, log_, serv, *this) {
}

void ares::account::session::defuse_asio() {
  close_socket();
  inactivity_timer_.cancel(); 
  if (is_client()) return as_client().defuse_asio();
  if (is_char_server()) return as_char_server().defuse_asio();
  if (is_mono()) return as_mono().defuse_asio();
}

void ares::account::session::remove_from_server() {
  defuse_asio();
  server_.remove(shared_from_this());
}

void ares::account::session::on_disconnect() {
  if (is_client()) {
    // Do not destroy client session if it has disconnected
    // The session data persist with authentication info
    inactivity_timer_.cancel();
    // TODO: Set inactivity timer to self-destruct after some period?
    return;
  } else {
    server_.remove(shared_from_this());
  }
}

void ares::account::session::on_open() {
}

void ares::account::session::before_close() {
}

void ares::account::session::on_connection_reset() {
  SPDLOG_TRACE(log_, "account::session:on_connection_reset");
  on_disconnect();
}

void ares::account::session::on_eof() {
  SPDLOG_TRACE(log_, "account::session:on_eof");
  on_disconnect();
}

void ares::account::session::on_socket_error() {
  SPDLOG_TRACE(log_, "account::session:on_socket_error");
  remove_from_server();
}

void ares::account::session::on_operation_aborted() {
  SPDLOG_TRACE(log_, "account::session:on_operation_aborted");
}

auto ares::account::session::state_variant() -> state_variant_type& {
  return state_;
}

bool ares::account::session::is_client() const {
  return std::holds_alternative<client::state>(state_);
}

auto ares::account::session::as_client() -> client::state& {
  return std::get<client::state>(state_);
}

bool ares::account::session::is_char_server() const {
  return std::holds_alternative<char_server::state>(state_);
}

auto ares::account::session::as_char_server() -> char_server::state& {
  return std::get<char_server::state>(state_);
}

bool ares::account::session::is_mono() const {
  return std::holds_alternative<mono::state>(state_);
}

auto ares::account::session::as_mono() -> mono::state& {
  return std::get<mono::state>(state_);
}

auto ares::account::session::make_send_handler() -> send_handler {
  return send_handler(shared_from_this());
}

auto ares::account::session::make_recv_handler() -> recv_handler {
  return recv_handler(shared_from_this());
}

