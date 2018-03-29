#include "state.hpp"

#include "../server.hpp"
#include "../session.hpp"

#include "../mono/state.hpp"

ares::zone::client::state::state(ares::zone::server& serv,
                                 ares::zone::session& sess) :
  server_(serv),
  session_(sess) {
  }

ares::zone::client::state::state(const ares::zone::mono::state& mono_state) :
  account_id(mono_state.account_id),
  character_id(mono_state.character_id),
  auth_code1(mono_state.auth_code1),
  client_time_diff(mono_state.client_time_diff),
  server_(mono_state.server_),
  session_(mono_state.session_),
  obf_crypt_key_(mono_state.obf_crypt_key_) {
}

void ares::zone::client::state::on_connect() {
}

void ares::zone::client::state::on_connection_reset() {
  session_.close_abruptly();
}

void ares::zone::client::state::on_operation_aborted() {
  session_.close_abruptly();
}

void ares::zone::client::state::on_eof() {
  session_.close_abruptly();
}

void ares::zone::client::state::on_socket_error() {
  session_.close_abruptly();
}

void ares::zone::client::state::rotate_obf_crypt_key() {
  // Rotate packet id obfuscation key
  SPDLOG_TRACE(log(), "client::state::rotate_obf_crypt_key");
  if (obf_crypt_key_) {
    obf_crypt_key_.emplace(*obf_crypt_key_ *
                           std::get<1>(*server_.conf().obfuscation_key) +
                           std::get<2>(*server_.conf().obfuscation_key));
  }
}

void ares::zone::client::state::defuse_asio() {
}

auto ares::zone::client::state::packet_sizes(uint16_t& packet_id) -> std::tuple<size_t, size_t, size_t> {
  SPDLOG_TRACE(log(), "client::state::allocate packet_id before applying obf decrypt {:#x}", packet_id);
  if (obf_crypt_key_) packet_id = packet_id ^ ((*obf_crypt_key_ >> 16) & 0x7fff);
  switch (packet_id) {
    ARES_PACKET_SIZES_CASE(CZ_LESSEFFECT);
    ARES_PACKET_SIZES_CASE(CZ_NOTIFY_ACTORINIT);
    ARES_PACKET_SIZES_CASE(CZ_REQUEST_TIME);
    ARES_PACKET_SIZES_CASE(CZ_REQ_SCHEDULER_CASHITEM);
    ARES_PACKET_SIZES_CASE(CZ_REQ_GUILD_MENU);
    ARES_PACKET_SIZES_CASE(CZ_REQNAME);
    ARES_PACKET_SIZES_CASE(CZ_REQUEST_MOVE);
  default:
    {
      log()->error("Unexpected packet_id {:#x} for client session while getting packet sizes", packet_id);
      return std::tuple<size_t, size_t, size_t>(0, 0, 0);
    }
  }
}

void ares::zone::client::state::dispatch_packet(std::shared_ptr<std::byte[]> buf) {
  uint16_t* packet_id = reinterpret_cast<uint16_t*>(buf.get());
  switch (*packet_id) {
    ARES_DISPATCH_PACKET_CASE(CZ_LESSEFFECT);
    ARES_DISPATCH_PACKET_CASE(CZ_NOTIFY_ACTORINIT);
    ARES_DISPATCH_PACKET_CASE(CZ_REQUEST_TIME);
    ARES_DISPATCH_PACKET_CASE(CZ_REQ_SCHEDULER_CASHITEM);
    ARES_DISPATCH_PACKET_CASE(CZ_REQ_GUILD_MENU);
    ARES_DISPATCH_PACKET_CASE(CZ_REQNAME);
    ARES_DISPATCH_PACKET_CASE(CZ_REQUEST_MOVE);
  default:
    {
      log()->error("Unexpected packet_id {:#x} for client::state session while dispatching, disconnecting", *packet_id);
      session_.close_gracefuly();
      return;
    }
  }
  rotate_obf_crypt_key();
}

auto ares::zone::client::state::log() const -> std::shared_ptr<spdlog::logger> {
  return server_.log();
}

auto ares::zone::client::state::conf() const -> const config& {
  return server_.conf();
}
