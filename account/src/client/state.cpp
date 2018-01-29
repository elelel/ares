#include "state.hpp"

#include "../state.hpp"

ares::account::client::state::state(account::state& server_state, session& sess) :
  server_state_(server_state),
  session_(sess) {
  }

ares::account::client::state::state(const mono::state& mono_state) :
  server_state_(mono_state.server_state_),
  session_(mono_state.session_),
  client_exe_hash(mono_state.client_exe_hash) {
}

void ares::account::client::state::defuse_asio() {
}

void ares::account::client::state::on_open() {
  SPDLOG_TRACE(server_state_.log(), "client::state on_open");
}

void ares::account::client::state::before_close() {
  SPDLOG_TRACE(server_state_.log(), "client::state before_close");
}

void ares::account::client::state::on_connection_reset() {
  SPDLOG_TRACE(server_state_.log(), "client::state on_connection_reset");
}

void ares::account::client::state::on_eof() {
  SPDLOG_TRACE(server_state_.log(), "client::state on_eof");
}

void ares::account::client::state::on_socket_error() {
  SPDLOG_TRACE(server_state_.log(), "client::state on_socket_error");
}

void ares::account::client::state::on_operation_aborted() {
  SPDLOG_TRACE(server_state_.log(), "client::state on_operation_aborted");
}

size_t ares::account::client::state::dispatch(const uint16_t PacketType) {
  SPDLOG_TRACE(server_state_.log(), "client::state::dispatch() switching on PacketType = {0:#x}", PacketType);
  switch (PacketType) {
  default:
    server_state_.log()->error("Unexpected PacketType {0:#x} for client::state session", PacketType);
    throw network::terminate_session();
  }
}
