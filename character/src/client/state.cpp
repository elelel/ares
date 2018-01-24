#include "state.hpp"

#include <ares/packets>
#include <ares/network>

#include "../server.hpp"
#include "packet_handlers.hpp"

ares::character::client::state::state(std::shared_ptr<spdlog::logger> log, server& serv, session& sess) :
  log_(log),
  server_(serv),
  session_(sess) {
  }

ares::character::client::state::state(const mono::state& mono_state) :
  log_(mono_state.log_),
  server_(mono_state.server_),
  session_(mono_state.session_) {
}

void ares::character::client::state::defuse_asio() {
}

void ares::character::client::state::on_open() {
  SPDLOG_TRACE(log_, "client::state on_open");
}

void ares::character::client::state::before_close() {
  SPDLOG_TRACE(log_, "client::state before_close");
}

void ares::character::client::state::on_connection_reset() {
  SPDLOG_TRACE(log_, "client::state on_connection_reset");
}

void ares::character::client::state::on_eof() {
  SPDLOG_TRACE(log_, "client::state on_eof");
}

void ares::character::client::state::on_socket_error() {
  SPDLOG_TRACE(log_, "client::state on_socket_error");
}

void ares::character::client::state::on_operation_aborted() {
  SPDLOG_TRACE(log_, "client::state on_operation_aborted");
}

size_t ares::character::client::state::dispatch(const uint16_t PacketType) {
  SPDLOG_TRACE(log_, "client::state::dispatch() switching on PacketType = {0:#x}", PacketType);
  switch (PacketType) {
    ARES_DISPATCH_PACKET_CASE(PING);
    ARES_DISPATCH_PACKET_CASE(CH_MAKE_CHAR);
    ARES_DISPATCH_PACKET_CASE(CH_SELECT_CHAR);
    ARES_DISPATCH_PACKET_CASE(CH_CHAR_PAGE_REQ);
  }
  SPDLOG_TRACE(log_, "client::state::dispatch() done");
  log_->error("Unexpected PacketType {0:#x} for client::state session", PacketType);
  throw ares::network::terminate_session();
}

