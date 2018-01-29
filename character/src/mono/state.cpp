#include "state.hpp"

#include <ares/packets>
#include <ares/network>

#include "../state.hpp"

ares::character::mono::state::state(character::state& server_state, session& sess) :
  server_state_(server_state),
  session_(sess) {
  }

void ares::character::mono::state::defuse_asio() {
}

void ares::character::mono::state::on_open() {
  SPDLOG_TRACE(server_state_.log(), "mono::state on_open");
}

void ares::character::mono::state::before_close() {
  SPDLOG_TRACE(server_state_.log(), "mono::state before_close");
}

void ares::character::mono::state::on_connection_reset() {
  SPDLOG_TRACE(server_state_.log(), "mono::state on_connection_reset");
}

void ares::character::mono::state::on_eof() {
  SPDLOG_TRACE(server_state_.log(), "mono::state on_eof");
}

void ares::character::mono::state::on_socket_error() {
  SPDLOG_TRACE(server_state_.log(), "mono::state on_socket_error");
}

void ares::character::mono::state::on_operation_aborted() {
  SPDLOG_TRACE(server_state_.log(), "mono::state on_operation_aborted");
}

size_t ares::character::mono::state::dispatch(const uint16_t PacketType) {
  SPDLOG_TRACE(server_state_.log(), "mono::state::dispatch() switching on PacketType = {0:#x}", PacketType);
  switch (PacketType) {
    ARES_PACKET_CASE(CH_ENTER);
    ARES_PACKET_CASE(ATHENA_ZH_LOGIN_REQ);
  }
  server_state_.log()->error("Unexpected PacketType {0:#x} for mono::state session", PacketType);
  throw ares::network::terminate_session();
}
