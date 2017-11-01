#include "state.hpp"

#include <ares/packets>
#include <ares/network>

#include "../server.hpp"
#include "packet_handlers.hpp"

ares::zone::mono::state::state(std::shared_ptr<spdlog::logger> log, server& serv, session& sess) :
  log_(log),
  server_(serv),
  session_(sess) {
  }

void ares::zone::mono::state::defuse_asio() {
}

void ares::zone::mono::state::on_open() {
  SPDLOG_TRACE(log_, "mono::state on_open");
}

void ares::zone::mono::state::before_close() {
  SPDLOG_TRACE(log_, "mono::state before_close");
}

void ares::zone::mono::state::on_connection_reset() {
  SPDLOG_TRACE(log_, "mono::state on_connection_reset");
}

void ares::zone::mono::state::on_eof() {
  SPDLOG_TRACE(log_, "mono::state on_eof");
}

void ares::zone::mono::state::on_socket_error() {
  SPDLOG_TRACE(log_, "mono::state on_socket_error");
}

void ares::zone::mono::state::on_operation_aborted() {
  SPDLOG_TRACE(log_, "mono::state on_operation_aborted");
}

size_t ares::zone::mono::state::dispatch(const uint16_t PacketType) {
  SPDLOG_TRACE(log_, "mono::state::dispatch() switching on PacketType = {0:#x}", PacketType);
  switch (PacketType) {

  }
  log_->error("Unexpected PacketType {0:#x} for mono::state session", PacketType);
  throw ares::network::terminate_session();
}

