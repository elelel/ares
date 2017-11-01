#include "state.hpp"

#include <ares/packets>
#include <ares/network>

#include "../server.hpp"
#include "packet_handlers.hpp"

ares::character::mono::state::state(std::shared_ptr<spdlog::logger> log, server& serv, session& sess) :
  log_(log),
  server_(serv),
  session_(sess) {
  }

void ares::character::mono::state::defuse_asio() {
}

void ares::character::mono::state::on_open() {
  SPDLOG_TRACE(log_, "mono::state on_open");
}

void ares::character::mono::state::before_close() {
  SPDLOG_TRACE(log_, "mono::state before_close");
}

void ares::character::mono::state::on_connection_reset() {
  SPDLOG_TRACE(log_, "mono::state on_connection_reset");
}

void ares::character::mono::state::on_eof() {
  SPDLOG_TRACE(log_, "mono::state on_eof");
}

void ares::character::mono::state::on_socket_error() {
  SPDLOG_TRACE(log_, "mono::state on_socket_error");
}

void ares::character::mono::state::on_operation_aborted() {
  SPDLOG_TRACE(log_, "mono::state on_operation_aborted");
}

size_t ares::character::mono::state::dispatch(const uint16_t PacketType) {
  SPDLOG_TRACE(log_, "mono::state::dispatch() switching on PacketType = {0:#x}", PacketType);
  switch (PacketType) {
    ARES_DISPATCH_PACKET_CASE(CH_ENTER);
    ARES_DISPATCH_PACKET_CASE(ATHENA_ZH_LOGIN_REQ);
  }
  log_->error("Unexpected PacketType {0:#x} for mono::state session", PacketType);
  throw ares::network::terminate_session();
}
