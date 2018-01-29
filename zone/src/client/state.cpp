#include "state.hpp"

#include <ares/network>

#include "../state.hpp"

ares::zone::client::state::state(zone::state& server_state, session& sess) :
  server_state_(server_state),
  session_(sess) {
  }

void ares::zone::client::state::defuse_asio() {
}

void ares::zone::client::state::on_open() {
  SPDLOG_TRACE(server_state_.log(), "client::state on_open");
}

void ares::zone::client::state::before_close() {
  SPDLOG_TRACE(server_state_.log(), "client::state before_close");
}

void ares::zone::client::state::on_connection_reset() {
  SPDLOG_TRACE(server_state_.log(), "client::state on_connection_reset");
}

void ares::zone::client::state::on_eof() {
  SPDLOG_TRACE(server_state_.log(), "client::state on_eof");
}

void ares::zone::client::state::on_socket_error() {
  SPDLOG_TRACE(server_state_.log(), "client::state on_socket_error");
}

void ares::zone::client::state::on_operation_aborted() {
  SPDLOG_TRACE(server_state_.log(), "client::state on_operation_aborted");
}

size_t ares::zone::client::state::dispatch(const uint16_t PacketType) {
  SPDLOG_TRACE(server_state_.log(), "client::state::dispatch() switching on PacketType = {0:#x}", PacketType);
  switch (PacketType) {
    ARES_PACKET_CASE(CZ_REQUEST_MOVE);
  }
  SPDLOG_TRACE(server_state_.log(), "client::state::dispatch() done");
  server_state_.log()->error("Unexpected PacketType {0:#x} for client::state session", PacketType);
  throw ares::network::terminate_session();
}
