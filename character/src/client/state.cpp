#include "state.hpp"
#include "../server.hpp"

ares::character::client::state::state(character::server& serv, session& sess) :
  server_(serv),
  session_(sess) {
  }

ares::character::client::state::state(const mono::state& mono_state) :
  auth_code1(mono_state.auth_code1),
  auth_code2(mono_state.auth_code2),
  server_(mono_state.server_),
  session_(mono_state.session_) {
}

ares::character::client::state::~state() {
  SPDLOG_TRACE(log(), "Destructing client state for session {}", session_.id());
}

void ares::character::client::state::on_connect() {
}

void ares::character::client::state::on_connection_reset() {
  session_.close_abruptly();
}

void ares::character::client::state::on_operation_aborted() {
  session_.close_abruptly();
}

void ares::character::client::state::on_eof() {
  SPDLOG_TRACE(log(), "Client session {} EOF", session_.id());
}

void ares::character::client::state::on_socket_error() {
  session_.close_abruptly();
}

void ares::character::client::state::defuse_asio() {
}

auto ares::character::client::state::packet_sizes(const uint16_t packet_id) -> std::tuple<size_t, size_t, size_t> {
  switch (packet_id) {
    ARES_PACKET_SIZES_CASE(PING);
    ARES_PACKET_SIZES_CASE(CH_MAKE_CHAR::no_stats);
    ARES_PACKET_SIZES_CASE(CH_SELECT_CHAR);
    ARES_PACKET_SIZES_CASE(CH_CHAR_PAGE_REQ);
  default:
    { 
      log()->error("Unexpected packet_id {:#x} for client session while getting packet sizes", packet_id);
      return std::tuple<size_t, size_t, size_t>(0, 0, 0);
    }
  }
}

void ares::character::client::state::dispatch_packet(std::shared_ptr<std::byte[]> buf) {
  uint16_t* packet_id = reinterpret_cast<uint16_t*>(buf.get());
  switch (*packet_id) {
    ARES_DISPATCH_PACKET_CASE(PING);
    ARES_DISPATCH_PACKET_CASE(CH_MAKE_CHAR::no_stats);
    ARES_DISPATCH_PACKET_CASE(CH_SELECT_CHAR);
    ARES_DISPATCH_PACKET_CASE(CH_CHAR_PAGE_REQ);
  default:
    {
      log()->error("Unexpected packet_id {:#x} for client session while dispatching, disconnecting", *packet_id);
      session_.close_gracefuly();
      return;
    }
  }
}

auto ares::character::client::state::log() const -> std::shared_ptr<spdlog::logger> {
  return server_.log();
}

auto ares::character::client::state::conf() const -> const config& {
  return server_.conf();
}
