#include "state.hpp"

#include <ares/packets>
#include <ares/network>

#include "../server.hpp"
#include "packet_handlers.hpp"

ares::account::mono::state::state(std::shared_ptr<spdlog::logger> log, server& serv, session& sess) :
  log_(log),
  server_(serv),
  session_(sess) {
  }

void ares::account::mono::state::defuse_asio() {
}

void ares::account::mono::state::on_open() {
  SPDLOG_TRACE(log_, "mono::state on_open");
}

void ares::account::mono::state::before_close() {
  SPDLOG_TRACE(log_, "mono::state before_close");
}

void ares::account::mono::state::on_connection_reset() {
  SPDLOG_TRACE(log_, "mono::state on_connection_reset");
}

void ares::account::mono::state::on_eof() {
  SPDLOG_TRACE(log_, "mono::state on_eof");
}

void ares::account::mono::state::on_socket_error() {
  SPDLOG_TRACE(log_, "mono::state on_socket_error");
}

void ares::account::mono::state::on_operation_aborted() {
  SPDLOG_TRACE(log_, "mono::state on_operation_aborted");
}

size_t ares::account::mono::state::dispatch(const uint16_t PacketType) {
  SPDLOG_TRACE(log_, "mono::state::dispatch() switching on PacketType = {0:#x}", PacketType);
  switch (PacketType) {
    ARES_DISPATCH_PACKET_CASE(CA_EXE_HASHCHECK);
    ARES_DISPATCH_PACKET_CASE(CA_SSO_LOGIN_REQ_0x64);
    ARES_DISPATCH_PACKET_CASE(CA_SSO_LOGIN_REQ);
    ARES_DISPATCH_PACKET_CASE(ATHENA_HA_LOGIN_REQ);
    ARES_DISPATCH_PACKET_CASE(ATHENA_HA_PING_REQ);
  default:
    log_->error("Unexpected PacketType {0:#x} for mono::state session", PacketType);
    throw ares::network::terminate_session();
  }
  SPDLOG_TRACE(log_, "mono::state::dispatch() done");
}
