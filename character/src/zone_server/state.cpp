#include "state.hpp"

#include <ares/packets>
#include <ares/network>

#include "../state.hpp"

ares::character::zone_server::state::state(character::state& server_state, session& sess) :
  server_state_(server_state),
  session_(sess) {
  }

ares::character::zone_server::state::state(const mono::state& mono_state) :
  server_state_(mono_state.server_state_),
  session_(mono_state.session_) {
}

void ares::character::zone_server::state::defuse_asio() {
}

void ares::character::zone_server::state::on_open() {
  SPDLOG_TRACE(server_state_.log(), "zone_server::state on_open");
}

void ares::character::zone_server::state::before_close() {
  SPDLOG_TRACE(server_state_.log(), "zone_server::state before_close");
}

void ares::character::zone_server::state::on_connection_reset() {
  SPDLOG_TRACE(server_state_.log(), "zone_server::state on_connection_reset");
}

void ares::character::zone_server::state::on_eof() {
  SPDLOG_TRACE(server_state_.log(), "zone_server::state on_eof");
}

void ares::character::zone_server::state::on_socket_error() {
  SPDLOG_TRACE(server_state_.log(), "zone_server::state on_socket_error");
}

void ares::character::zone_server::state::on_operation_aborted() {
  SPDLOG_TRACE(server_state_.log(), "zone_server::state on_operation_aborted");
}

size_t ares::character::zone_server::state::dispatch(const uint16_t PacketType) {
  SPDLOG_TRACE(server_state_.log(), "zone_server::state::dispatch() switching on PacketType = {0:#x}", PacketType);
  switch (PacketType) {
    ARES_PACKET_CASE(ATHENA_ZH_ONLINE_USERS);
    ARES_PACKET_CASE(ATHENA_ZH_USER_COUNT);
    ARES_PACKET_CASE(ATHENA_ZH_GAME_RATES);
    ARES_PACKET_CASE(ATHENA_ZH_PING_REQ);
    ARES_PACKET_CASE(ARES_ZH_MAP_NAMES_REQ);
  }
  server_state_.log()->error("Unexpected PacketType {0:#x} for zone_server::state session", PacketType);
  throw ares::network::terminate_session();
}
