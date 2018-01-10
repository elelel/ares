#include "state.hpp"

#include <ares/packets>
#include <ares/network>

#include "../server.hpp"
#include "packet_handlers.hpp"

ares::zone::client::state::state(std::shared_ptr<spdlog::logger> log, server& serv, session& sess) :
  log_(log),
  server_(serv),
  session_(sess) {
  }

void ares::zone::client::state::defuse_asio() {
}

void ares::zone::client::state::on_open() {
  SPDLOG_TRACE(log_, "client::state on_open");
}

void ares::zone::client::state::before_close() {
  SPDLOG_TRACE(log_, "client::state before_close");
}

void ares::zone::client::state::on_connection_reset() {
  SPDLOG_TRACE(log_, "client::state on_connection_reset");
}

void ares::zone::client::state::on_eof() {
  SPDLOG_TRACE(log_, "client::state on_eof");
}

void ares::zone::client::state::on_socket_error() {
  SPDLOG_TRACE(log_, "client::state on_socket_error");
}

void ares::zone::client::state::on_operation_aborted() {
  SPDLOG_TRACE(log_, "client::state on_operation_aborted");
}

size_t ares::zone::client::state::dispatch(const uint16_t PacketType) {
  SPDLOG_TRACE(log_, "client::state::dispatch() switching on PacketType = {0:#x}", PacketType);
  switch (PacketType) {
    ARES_DISPATCH_PACKET_CASE(CZ_REQUEST_MOVE);
  }
  SPDLOG_TRACE(log_, "client::state::dispatch() done");
  log_->error("Unexpected PacketType {0:#x} for client::state session", PacketType);
  throw ares::network::terminate_session();
}

