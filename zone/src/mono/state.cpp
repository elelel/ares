#include "state.hpp"

#include <ares/packets>
#include <ares/network>

#include "../state.hpp"

ares::zone::mono::state::state(zone::state& zone_state, session& sess) :
  server_state_(zone_state),
  session_(sess) {
  }

void ares::zone::mono::state::defuse_asio() {
}

void ares::zone::mono::state::on_open() {
  SPDLOG_TRACE(server_state_.log(), "mono::state on_open");
}

void ares::zone::mono::state::before_close() {
  SPDLOG_TRACE(server_state_.log(), "mono::state before_close");
}

void ares::zone::mono::state::on_connection_reset() {
  SPDLOG_TRACE(server_state_.log(), "mono::state on_connection_reset");
}

void ares::zone::mono::state::on_eof() {
  SPDLOG_TRACE(server_state_.log(), "mono::state on_eof");
}

void ares::zone::mono::state::on_socket_error() {
  SPDLOG_TRACE(server_state_.log(), "mono::state on_socket_error");
}

void ares::zone::mono::state::on_operation_aborted() {
  SPDLOG_TRACE(server_state_.log(), "mono::state on_operation_aborted");
}

size_t ares::zone::mono::state::dispatch(const uint16_t PacketType) {
  SPDLOG_TRACE(server_state_.log(), "mono::state::dispatch() switching on PacketType = {0:#x}", PacketType);
  switch (PacketType) {
    ARES_PACKET_CASE(CZ_ENTER);
  }
  server_state_.log()->error("Unexpected PacketType {0:#x} for mono::state session", PacketType);
  throw ares::network::terminate_session();
}

