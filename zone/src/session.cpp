#include "session.hpp"

#include "server.hpp"

#include <utility>

#include "mono/state.hpp"

ares::zone::session::session(server& serv,
                                std::shared_ptr<boost::asio::ip::tcp::socket> socket) :
  ares::network::session<session>(serv.io_service(), serv.log(), socket),
  server_(serv),
  state_(std::in_place_type<mono::state>, log_, serv, *this) {
}

void ares::zone::session::remove_from_server() {
  close_socket();
  inactivity_timer_.cancel();
  server_.remove(shared_from_this());
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

auto ares::zone::session::state_variant() -> state_variant_type& {
  return state_;
}

bool ares::zone::session::is_client() const {
  return std::holds_alternative<client::state>(state_);
}

auto ares::zone::session::as_client() -> client::state& {
  return std::get<client::state>(state_);
}

bool ares::zone::session::is_character_server() const {
  return std::holds_alternative<character_server::state>(state_);
}

auto ares::zone::session::as_character_server() -> character_server::state& {
  return std::get<character_server::state>(state_);
}

auto ares::zone::session::make_send_handler() -> send_handler {
  return send_handler(shared_from_this());
}

auto ares::zone::session::make_recv_handler() -> recv_handler {
  return recv_handler(shared_from_this());

}
