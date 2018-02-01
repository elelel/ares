#include "state.hpp"
#include "../state.hpp"

ares::character::client::state::state(character::state& server_state, session& sess) :
  server_state_(server_state),
  session_(sess) {
  }

ares::character::client::state::state(const mono::state& mono_state) :
  server_state_(mono_state.server_state_),
  session_(mono_state.session_) {
}

void ares::character::client::state::defuse_asio() {
}

void ares::character::client::state::on_open() {
  SPDLOG_TRACE(server_state_.log(), "client::state on_open");
}

void ares::character::client::state::before_close() {
  SPDLOG_TRACE(server_state_.log(), "client::state before_close");
}

void ares::character::client::state::on_connection_reset() {
  SPDLOG_TRACE(server_state_.log(), "client::state on_connection_reset");
}

void ares::character::client::state::on_eof() {
  SPDLOG_TRACE(server_state_.log(), "client::state on_eof");
}

void ares::character::client::state::on_socket_error() {
  SPDLOG_TRACE(server_state_.log(), "client::state on_socket_error");
}

void ares::character::client::state::on_operation_aborted() {
  SPDLOG_TRACE(server_state_.log(), "client::state on_operation_aborted");
}

size_t ares::character::client::state::dispatch(const uint16_t PacketType) {
  SPDLOG_TRACE(server_state_.log(), "client::state::dispatch() switching on PacketType = {0:#x}", PacketType);
  switch (PacketType) {
    ARES_PACKET_CASE(PING);
    ARES_PACKET_CASE(CH_MAKE_CHAR::no_stats);
    ARES_PACKET_CASE(CH_SELECT_CHAR);
    ARES_PACKET_CASE(CH_CHAR_PAGE_REQ);
  }
  SPDLOG_TRACE(server_state_.log(), "client::state::dispatch() done");
  server_state_.log()->error("Unexpected PacketType {0:#x} for client::state session", PacketType);
  throw ares::network::terminate_session();
}

