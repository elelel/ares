#include "state.hpp"

#include <ares/packets>
#include <ares/network>

#include "../state.hpp"

ares::zone::character_server::state::state(zone::state& server_state, session& sess) :
  server_state_(server_state),
  session_(sess),
  reconnect_timer(sess.shared_from_this(), std::chrono::seconds(5)),
  ping_request_timer(sess.shared_from_this(), std::chrono::seconds(10)),
  ping_timeout_timer(sess.shared_from_this(), std::chrono::seconds(5)) {
  }

void ares::zone::character_server::state::defuse_asio() {
  reconnect_timer.cancel();
  ping_request_timer.cancel();
  ping_timeout_timer.cancel();
}

void ares::zone::character_server::state::on_open() {
  SPDLOG_TRACE(server_state_.log(), "character_server::state on_open");
}

void ares::zone::character_server::state::before_close() {
  SPDLOG_TRACE(server_state_.log(), "character_server::state before_close");
}

void ares::zone::character_server::state::on_connection_reset() {
  server_state_.log()->warn("Connection with account server reset");
  SPDLOG_TRACE(server_state_.log(), "character_server::state on_connection_reset");
}

void ares::zone::character_server::state::on_eof() {
  server_state_.log()->warn("Connection with account server closed");
  SPDLOG_TRACE(server_state_.log(), "character_server::state on_eof");
}

void ares::zone::character_server::state::on_socket_error() {
  SPDLOG_TRACE(server_state_.log(), "character_server::state on_socket_error");
}

void ares::zone::character_server::state::on_operation_aborted() {
  SPDLOG_TRACE(server_state_.log(), "character_server::state on_operation_aborted");
}

size_t ares::zone::character_server::state::dispatch(const uint16_t PacketType) {
  SPDLOG_TRACE(server_state_.log(), "character_server::state::dispatch() switching on PacketType = {0:#x}", PacketType);
  switch (PacketType) {
    ARES_PACKET_CASE(ATHENA_HZ_LOGIN_RESULT);
    ARES_PACKET_CASE(ATHENA_HZ_PRIVATE_MSG_NAME);
    ARES_PACKET_CASE(ATHENA_HZ_PING_ACK);
    ARES_PACKET_CASE(ARES_HZ_MAP_NAMES);
  }
  server_state_.log()->error("Unexpected PacketType {0:#x} for character_server::state session", PacketType);
  throw ares::network::terminate_session();
}
