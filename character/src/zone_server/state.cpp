#include "state.hpp"

#include <ares/packets>
#include <ares/network>

#include "../server.hpp"
#include "packet_handlers.hpp"

ares::character::zone_server::state::state(std::shared_ptr<spdlog::logger> log, server& serv, session& sess) :
  log_(log),
  server_(serv),
  session_(sess) {
  }

ares::character::zone_server::state::state(const mono::state& mono_state) :
  log_(mono_state.log_),
  server_(mono_state.server_),
  session_(mono_state.session_) {
}

void ares::character::zone_server::state::defuse_asio() {
}

void ares::character::zone_server::state::on_open() {
  SPDLOG_TRACE(log_, "zone_server::state on_open");
}

void ares::character::zone_server::state::before_close() {
  SPDLOG_TRACE(log_, "zone_server::state before_close");
}

void ares::character::zone_server::state::on_connection_reset() {
  SPDLOG_TRACE(log_, "zone_server::state on_connection_reset");
}

void ares::character::zone_server::state::on_eof() {
  SPDLOG_TRACE(log_, "zone_server::state on_eof");
}

void ares::character::zone_server::state::on_socket_error() {
  SPDLOG_TRACE(log_, "zone_server::state on_socket_error");
}

void ares::character::zone_server::state::on_operation_aborted() {
  SPDLOG_TRACE(log_, "zone_server::state on_operation_aborted");
}

size_t ares::character::zone_server::state::dispatch(const uint16_t PacketType) {
  SPDLOG_TRACE(log_, "zone_server::state::dispatch() switching on PacketType = {0:#x}", PacketType);
  switch (PacketType) {
    ARES_DISPATCH_PACKET_CASE(ATHENA_ZH_MAP_NAMES);
    ARES_DISPATCH_PACKET_CASE(ATHENA_ZH_ONLINE_USERS);
    ARES_DISPATCH_PACKET_CASE(ATHENA_ZH_USER_COUNT);
    ARES_DISPATCH_PACKET_CASE(ATHENA_ZH_GAME_RATES);
    ARES_DISPATCH_PACKET_CASE(ATHENA_ZH_PING_REQ);
  }
  log_->error("Unexpected PacketType {0:#x} for zone_server::state session", PacketType);
  throw ares::network::terminate_session();
}
