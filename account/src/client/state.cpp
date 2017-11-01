#include "state.hpp"

#include "../server.hpp"

#include "../mono/state.hpp"
#include "packet_handlers.hpp"

ares::account::client::state::state(std::shared_ptr<spdlog::logger> log, server& serv, session& sess) :
  log_(log),
  server_(serv),
  session_(sess) {
  }

ares::account::client::state::state(const mono::state& mono_state) :
  log_(mono_state.log_),
  server_(mono_state.server_),
  session_(mono_state.session_),
  client_exe_hash(mono_state.client_exe_hash) {
}

void ares::account::client::state::defuse_asio() {
}

void ares::account::client::state::on_open() {
  SPDLOG_TRACE(log_, "client::state on_open");
}

void ares::account::client::state::before_close() {
  SPDLOG_TRACE(log_, "client::state before_close");
}

void ares::account::client::state::on_connection_reset() {
  SPDLOG_TRACE(log_, "client::state on_connection_reset");
}

void ares::account::client::state::on_eof() {
  SPDLOG_TRACE(log_, "client::state on_eof");
}

void ares::account::client::state::on_socket_error() {
  SPDLOG_TRACE(log_, "client::state on_socket_error");
}

void ares::account::client::state::on_operation_aborted() {
  SPDLOG_TRACE(log_, "client::state on_operation_aborted");
}

size_t ares::account::client::state::dispatch(const uint16_t PacketType) {
  SPDLOG_TRACE(log_, "client::state::dispatch() switching on PacketType = {0:#x}", PacketType);
  switch (PacketType) {
  default:
    log_->error("Unexpected PacketType {0:#x} for client::state session", PacketType);
    throw network::terminate_session();
  }
}
