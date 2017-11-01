#include "state.hpp"

#include "../server.hpp"
#include "packet_handlers.hpp"

ares::account::char_server::state::state(std::shared_ptr<spdlog::logger> log, server& serv, session& sess) :
  log_(log),
  server_(serv),
  session_(sess) {
  }

ares::account::char_server::state::state(const mono::state& mono_state) :
  log_(mono_state.log_),
  server_(mono_state.server_),
  session_(mono_state.session_) {
}

void ares::account::char_server::state::defuse_asio() {
}

void ares::account::char_server::state::on_open() {
  SPDLOG_TRACE(log_, "char_server::state on_open");
}

void ares::account::char_server::state::before_close() {
  SPDLOG_TRACE(log_, "char_server::state before_close");
}

void ares::account::char_server::state::on_connection_reset() {
  SPDLOG_TRACE(log_, "char_server::state on_connection_reset");
}

void ares::account::char_server::state::on_eof() {
  SPDLOG_TRACE(log_, "char_server::state on_eof");
}

void ares::account::char_server::state::on_socket_error() {
  SPDLOG_TRACE(log_, "char_server::state on_socket_error");
}

void ares::account::char_server::state::on_operation_aborted() {
  SPDLOG_TRACE(log_, "char_server::state on_operation_aborted");
}

size_t ares::account::char_server::state::dispatch(const uint16_t PacketType) {
  SPDLOG_TRACE(log_, "char_server::state::dispatch() switching on PacketType = {0:#x}", PacketType);
  switch (PacketType) {
    ARES_DISPATCH_PACKET_CASE(ATHENA_HA_PING_REQ);
    ARES_DISPATCH_PACKET_CASE(ATHENA_HA_ONLINE_AIDS);
    ARES_DISPATCH_PACKET_CASE(ATHENA_HA_USER_COUNT);
    ARES_DISPATCH_PACKET_CASE(ATHENA_HA_AID_AUTH_REQ);
    ARES_DISPATCH_PACKET_CASE(ATHENA_HA_ACCOUNT_DATA_REQ);
    ARES_DISPATCH_PACKET_CASE(ATHENA_HA_SET_AID_ONLINE);
    ARES_DISPATCH_PACKET_CASE(ATHENA_HA_SET_AID_OFFLINE);
  default:
    log_->error("Unexpected PacketType {0:#x} for char_server::state session", PacketType);
    throw network::terminate_session();
  }
}
