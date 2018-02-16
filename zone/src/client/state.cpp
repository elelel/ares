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
  server_(mono_state.server_),
  session_(mono_state.session_),
  obf_crypt_key_(mono_state.obf_crypt_key_) {
}

void ares::zone::client::state::on_connect() {
}

void ares::zone::client::state::on_connection_reset() {
  server_.close_abruptly(session_.shared_from_this());
}

void ares::zone::client::state::on_operation_aborted() {
  server_.close_abruptly(session_.shared_from_this());
}

void ares::zone::client::state::on_eof() {
  server_.close_abruptly(session_.shared_from_this());
}

void ares::zone::client::state::on_socket_error() {
  server_.close_abruptly(session_.shared_from_this());
}

void ares::zone::client::state::rotate_obf_crypt_key() {
  // Rotate packet id obfuscation key
  if (obf_crypt_key_) {
    obf_crypt_key_.emplace(*obf_crypt_key_ *
                           std::get<1>(*server_.conf().obfuscation_key) +
                           std::get<2>(*server_.conf().obfuscation_key));
  }
}

void ares::zone::client::state::defuse_asio() {
}

auto ares::zone::client::state::allocate(uint16_t& packet_id) -> packet::alloc_info {
  if (obf_crypt_key_) packet_id = packet_id ^ ((*obf_crypt_key_ >> 16) & 0x7fff);  
  switch (packet_id) {
  default:
    { // Packet id is not known to this server under selected packet set
      log()->error("Unexpected packet_id {:#x} for client session while allocating", packet_id);
      packet::alloc_info ai;
      ai.expected_packet_sz = 0;
      ai.buf = nullptr;
      ai.buf_sz = 0;
      ai.deallocator = [] (void*) {};
      ai.PacketLength_offset = 0;
      return std::move(ai);
    }
  }
}

void ares::zone::client::state::dispatch_packet(void* buf, std::function<void(void*)> /* deallocator */) {
  uint16_t* packet_id = reinterpret_cast<uint16_t*>(buf);
  switch (*packet_id) {
  default:
    {
      log()->error("Unexpected packet_id {:#x} for client::state session, disconnecting", *packet_id);
      server_.close_gracefuly(session_.shared_from_this());
      session_.connected_ = false;
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
