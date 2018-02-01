#include "state.hpp"

#include <ares/network>

#include "../state.hpp"

ares::account::mono::state::state(account::state& server_state, session& sess) :
  server_state_(server_state),
  session_(sess) {
  }

void ares::account::mono::state::defuse_asio() {
}

void ares::account::mono::state::on_open() {
  SPDLOG_TRACE(server_state_.log(), "mono::state on_open");
}

void ares::account::mono::state::before_close() {
  SPDLOG_TRACE(server_state_.log(), "mono::state before_close");
}

void ares::account::mono::state::on_connection_reset() {
  SPDLOG_TRACE(server_state_.log(), "mono::state on_connection_reset");
}

void ares::account::mono::state::on_eof() {
  SPDLOG_TRACE(server_state_.log(), "mono::state on_eof");
}

void ares::account::mono::state::on_socket_error() {
  SPDLOG_TRACE(server_state_.log(), "mono::state on_socket_error");
}

void ares::account::mono::state::on_operation_aborted() {
  SPDLOG_TRACE(server_state_.log(), "mono::state on_operation_aborted");
}

size_t ares::account::mono::state::dispatch(const uint16_t PacketType) {
  SPDLOG_TRACE(server_state_.log(), "mono::state::dispatch() switching on PacketType = {0:#x}", PacketType);
  switch (PacketType) {
    ARES_PACKET_CASE(CA_EXE_HASHCHECK);
    ARES_PACKET_CASE(CA_SSO_LOGIN_REQ::login_password);
    ARES_PACKET_CASE(CA_SSO_LOGIN_REQ::token_auth);
    ARES_PACKET_CASE(ATHENA_HA_LOGIN_REQ);
    ARES_PACKET_CASE(ATHENA_HA_PING_REQ);
  default:
    server_state_.log()->error("Unexpected PacketType {0:#x} for mono::state session", PacketType);
    throw ares::network::terminate_session();
  }
  SPDLOG_TRACE(server_state_.log(), "mono::state::dispatch() done");
}
