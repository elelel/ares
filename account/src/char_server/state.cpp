#include "state.hpp"

#include "../state.hpp"

ares::account::char_server::state::state(account::state& server_state, session& sess) :
  server_state_(server_state),
  session_(sess) {
  }

ares::account::char_server::state::state(const mono::state& mono_state) :
  server_state_(mono_state.server_state_),
  session_(mono_state.session_) {
}

void ares::account::char_server::state::defuse_asio() {
}

void ares::account::char_server::state::on_open() {
  SPDLOG_TRACE(server_state_.log(), "char_server::state on_open");
}

void ares::account::char_server::state::before_close() {
  SPDLOG_TRACE(server_state_.log(), "char_server::state before_close");
}

void ares::account::char_server::state::on_connection_reset() {
  SPDLOG_TRACE(server_state_.log(), "char_server::state on_connection_reset");
}

void ares::account::char_server::state::on_eof() {
  SPDLOG_TRACE(server_state_.log(), "char_server::state on_eof");
}

void ares::account::char_server::state::on_socket_error() {
  SPDLOG_TRACE(server_state_.log(), "char_server::state on_socket_error");
}

void ares::account::char_server::state::on_operation_aborted() {
  SPDLOG_TRACE(server_state_.log(), "char_server::state on_operation_aborted");
}

size_t ares::account::char_server::state::dispatch(const uint16_t PacketType) {
  SPDLOG_TRACE(server_state_.log(), "char_server::state::dispatch() switching on PacketType = {0:#x}", PacketType);
  switch (PacketType) {
    ARES_PACKET_CASE(ATHENA_HA_PING_REQ);
    ARES_PACKET_CASE(ATHENA_HA_ONLINE_AIDS);
    ARES_PACKET_CASE(ATHENA_HA_USER_COUNT);
    ARES_PACKET_CASE(ATHENA_HA_AID_AUTH_REQ);
    ARES_PACKET_CASE(ATHENA_HA_ACCOUNT_DATA_REQ);
    ARES_PACKET_CASE(ATHENA_HA_SET_AID_ONLINE);
    ARES_PACKET_CASE(ATHENA_HA_SET_AID_OFFLINE);
  default:
    server_state_.log()->error("Unexpected PacketType {0:#x} for char_server::state session", PacketType);
    throw network::terminate_session();
  }
}
