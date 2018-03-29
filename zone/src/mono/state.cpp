#include "state.hpp"

#include "../server.hpp"
#include "../session.hpp"

ares::zone::mono::state::state(ares::zone::server& serv,
                                           ares::zone::session& sess) :
  server_(serv),
  session_(sess) {
  if (server_.conf().obfuscation_key) {
    const auto& k = *server_.conf().obfuscation_key;
    obf_crypt_key_.emplace(std::get<0>(k) * std::get<1>(k) + std::get<2>(k));
    SPDLOG_TRACE(log(), "Set new mono session obf_crypt_key to {:x}", *obf_crypt_key_);
  }
  }

void ares::zone::mono::state::on_connect() {
}

void ares::zone::mono::state::on_connection_reset() {
  session_.close_abruptly();
}

void ares::zone::mono::state::on_operation_aborted() {
  session_.close_abruptly();
}

void ares::zone::mono::state::on_eof() {
  session_.close_abruptly();
}

void ares::zone::mono::state::on_socket_error() {
  session_.close_abruptly();
}

void ares::zone::mono::state::rotate_obf_crypt_key() {
  // Rotate packet id obfuscation key
  if (obf_crypt_key_) {
    obf_crypt_key_.emplace(*obf_crypt_key_ *
                           std::get<1>(*server_.conf().obfuscation_key) +
                           std::get<2>(*server_.conf().obfuscation_key));
  }
}

void ares::zone::mono::state::defuse_asio() {
}

auto ares::zone::mono::state::packet_sizes(uint16_t& packet_id) -> std::tuple<size_t, size_t, size_t> {
  if (obf_crypt_key_) packet_id = packet_id ^ ((*obf_crypt_key_ >> 16) & 0x7fff);
  switch (packet_id) {
    ARES_PACKET_SIZES_CASE(CZ_ENTER);
  default:
    { // Packet id is not known to this server under selected packet set
      log()->error("Unexpected packet_id {:#x} for mono session while getting packet sizes", packet_id);
      return std::tuple<size_t, size_t, size_t>(0, 0, 0);
    }
  }
}

void ares::zone::mono::state::dispatch_packet(std::shared_ptr<std::byte[]> buf) {
  uint16_t* packet_id = reinterpret_cast<uint16_t*>(buf.get());
  switch (*packet_id) {
    ARES_DISPATCH_PACKET_CASE(CZ_ENTER);
  default:
    {
      log()->error("Unexpected packet_id {:#x} for mono session while dispatching, disconnecting", *packet_id);
      session_.close_gracefuly();
      return;
    }
  }
  rotate_obf_crypt_key();
}

auto ares::zone::mono::state::log() const -> std::shared_ptr<spdlog::logger> {
  return server_.log();
}

auto ares::zone::mono::state::conf() const -> const config& {
  return server_.conf();
}
