#include "state.hpp"

#include "../server.hpp"
#include "../session.hpp"

ares::account::character_server::state::state(ares::account::server& serv,
                                           ares::account::session& sess) :
  server_(serv),
  session_(sess) {
  }

ares::account::character_server::state::state(const mono::state& mono_state) :
  server_(mono_state.server_),
  session_(mono_state.session_) {
}

void ares::account::character_server::state::on_connect() {
}

void ares::account::character_server::state::on_connection_reset() {
  log()->warn("Disconnected from character server (connection reset)");
  // TODO: set offline aids
  session_.close_abruptly();
}

void ares::account::character_server::state::on_operation_aborted() {
  log()->warn("Operation aborted while communicating with character server");
  session_.close_abruptly();
}

void ares::account::character_server::state::on_eof() {
  log()->warn("Character server closed connection (eof)");
  session_.close_abruptly();
}

void ares::account::character_server::state::on_socket_error() {
  log()->warn("Socket error while communicating with character server");
  session_.close_abruptly();
}

void ares::account::character_server::state::defuse_asio() {
}

auto ares::account::character_server::state::packet_sizes(const uint16_t packet_id) -> std::tuple<size_t, size_t, size_t> {
  switch (packet_id) {
    ARES_PACKET_SIZES_CASE(ARES_HA_PING_REQ);
    ARES_PACKET_SIZES_CASE(ARES_HA_ONLINE_ACCOUNTS);
    ARES_PACKET_SIZES_CASE(ARES_HA_USER_COUNT);
    ARES_PACKET_SIZES_CASE(ARES_HA_ACCOUNT_AUTH_REQ);
  default:
    {
      log()->error("Unexpected packet_id {:#x} for character server session while getting packet sizes", packet_id);
      return std::tuple<size_t, size_t, size_t>(0, 0, 0);
    }
  }
}

void ares::account::character_server::state::dispatch_packet(std::shared_ptr<std::byte[]> buf) {
  uint16_t* packet_id = reinterpret_cast<uint16_t*>(buf.get());
  switch (*packet_id) {
    ARES_DISPATCH_PACKET_CASE(ARES_HA_PING_REQ);
    ARES_DISPATCH_PACKET_CASE(ARES_HA_ONLINE_ACCOUNTS);
    ARES_DISPATCH_PACKET_CASE(ARES_HA_USER_COUNT);
    ARES_DISPATCH_PACKET_CASE(ARES_HA_ACCOUNT_AUTH_REQ);
  default:
    {
      log()->error("Unexpected packet_id {:#x} for character server session while dispatching, disconnecting", *packet_id);
      session_.close_gracefuly();
      return;
    }
  }
}

auto ares::account::character_server::state::log() const -> std::shared_ptr<spdlog::logger> {
  return server_.log();
}

auto ares::account::character_server::state::conf() const -> const config& {
  return server_.conf();
}
