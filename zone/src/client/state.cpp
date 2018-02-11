#include "state.hpp"

#include "../server.hpp"
#include "../session.hpp"

ares::zone::client::state::state(ares::zone::server& serv,
                                           ares::zone::session& sess) :
  server_(serv),
  session_(sess) {
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

void ares::zone::client::state::on_packet_processed() {
  if (session_.obf_crypt_key) {
    session_.obf_crypt_key.emplace(*session_.obf_crypt_key *
                                   std::get<1>(*server_.conf().obfuscation_key) +
                                   std::get<2>(*server_.conf().obfuscation_key));
  }
}

void ares::zone::client::state::defuse_asio() {
}

auto ares::zone::client::state::allocate(const uint16_t packet_id) -> packet::alloc_info {
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

void ares::zone::client::state::dispatch_packet(const uint16_t packet_id, void* /* buf */, std::function<void(void*)> /* deallocator */) {
  switch (packet_id) {
  default:
    {
      log()->error("Unexpected packet_id {:#x} for client::state session, disconnecting", packet_id);
      server_.close_gracefuly(session_.shared_from_this());
      session_.connected_ = false;
    }
  }
}

auto ares::zone::client::state::log() const -> std::shared_ptr<spdlog::logger> {
  return server_.log();
}

auto ares::zone::client::state::conf() const -> const config& {
  return server_.conf();
}
