#include "state.hpp"

#include <ares/packets>
#include <ares/network>

#include "../server.hpp"

ares::character::zone_server::state::state(character::server& serv, session& sess) :
  server_(serv),
  session_(sess) {
  }

ares::character::zone_server::state::state(const mono::state& mono_state) :
  server_(mono_state.server_),
  session_(mono_state.session_) {
}

void ares::character::zone_server::state::on_connect() {
}

void ares::character::zone_server::state::on_connection_reset() {
  session_.close_abruptly();
}

void ares::character::zone_server::state::on_operation_aborted() {
  session_.close_abruptly();
}

void ares::character::zone_server::state::on_eof() {
  session_.close_abruptly();
}

void ares::character::zone_server::state::on_socket_error() {
  session_.close_abruptly();
}

void ares::character::zone_server::state::defuse_asio() {
}

auto ares::character::zone_server::state::packet_sizes(const uint16_t packet_id) -> std::tuple<size_t, size_t, size_t> {
  switch (packet_id) {
    ARES_PACKET_SIZES_CASE(ARES_ZH_GAME_RATES);
    ARES_PACKET_SIZES_CASE(ARES_ZH_PING_REQ);
    ARES_PACKET_SIZES_CASE(ARES_ZH_MAP_IDS_REQ);
    ARES_PACKET_SIZES_CASE(ARES_ZH_CHAR_AUTH_REQ);
  default:
    {
      log()->error("Unexpected packet_id {:#x} for zone server session while getting packet sizes", packet_id);
      return std::tuple<size_t, size_t, size_t>(0, 0, 0);
    }
  }
}

void ares::character::zone_server::state::dispatch_packet(std::shared_ptr<std::byte[]> buf) {
  uint16_t* packet_id = reinterpret_cast<uint16_t*>(buf.get());
  switch (*packet_id) {
    ARES_DISPATCH_PACKET_CASE(ARES_ZH_GAME_RATES);
    ARES_DISPATCH_PACKET_CASE(ARES_ZH_PING_REQ);
    ARES_DISPATCH_PACKET_CASE(ARES_ZH_MAP_IDS_REQ);
    ARES_DISPATCH_PACKET_CASE(ARES_ZH_CHAR_AUTH_REQ);
  default:
    {
      log()->error("Unexpected packet_id {:#x} for zone server session while dispatching, disconnecting", *packet_id);
      session_.close_gracefuly();
      return;
    }
  }
}

auto ares::character::zone_server::state::log() const -> std::shared_ptr<spdlog::logger> {
  return server_.log();
}

auto ares::character::zone_server::state::conf() const -> const config& {
  return server_.conf();
}
