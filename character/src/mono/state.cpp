#include "state.hpp"

#include <ares/packets>
#include <ares/network>

#include "../server.hpp"

ares::character::mono::state::state(character::server& serv, session& sess) :
  server_(serv),
  session_(sess) {
  }

ares::character::mono::state::~state() {
  SPDLOG_TRACE(log(), "Destructing mono state for session {}", session_.id());
}

void ares::character::mono::state::on_connect() {
}

void ares::character::mono::state::on_connection_reset() {
  session_.close_abruptly();
}

void ares::character::mono::state::on_operation_aborted() {
  session_.close_abruptly();
}

void ares::character::mono::state::on_eof() {
  session_.close_abruptly();
}

void ares::character::mono::state::on_socket_error() {
  session_.close_abruptly();
}

void ares::character::mono::state::defuse_asio() {
}

auto ares::character::mono::state::packet_sizes(const uint16_t packet_id) -> std::tuple<size_t, size_t, size_t> {
  SPDLOG_TRACE(log(), "mono::state::allocate {:#x}", packet_id);
  switch (packet_id) {
    ARES_PACKET_SIZES_CASE(CH_ENTER);
    ARES_PACKET_SIZES_CASE(ARES_ZH_LOGIN_REQ);
  default:
    {
      log()->error("Unexpected packet_id {:#x} for mono session while getting packet sizes", packet_id);
      return std::tuple<size_t, size_t, size_t>(0, 0, 0);
    }
  }
}

void ares::character::mono::state::dispatch_packet(std::shared_ptr<std::byte[]> buf) {
  uint16_t* packet_id = reinterpret_cast<uint16_t*>(buf.get());
  SPDLOG_TRACE(log(), "mono::state::dispatch_packet {:#x}", *packet_id);
  switch (*packet_id) {
    ARES_DISPATCH_PACKET_CASE(CH_ENTER);
    ARES_DISPATCH_PACKET_CASE(ARES_ZH_LOGIN_REQ);
  default:
    {
      log()->error("Unexpected packet_id {:#x} for mono session while dispatching, disconnecting", *packet_id);
      session_.close_gracefuly();
      return;
    }
  }
}

auto ares::character::mono::state::log() const -> std::shared_ptr<spdlog::logger> {
  return server_.log();
}

auto ares::character::mono::state::conf() const -> const config& {
  return server_.conf();
}
