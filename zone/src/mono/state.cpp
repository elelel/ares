#include "state.hpp"

#include "../server.hpp"
#include "../session.hpp"

ares::zone::mono::state::state(ares::zone::server& serv,
                                           ares::zone::session& sess) :
  server_(serv),
  session_(sess) {
  }

void ares::zone::mono::state::on_connect() {
}

void ares::zone::mono::state::on_connection_reset() {
  server_.close_abruptly(session_.shared_from_this());
}

void ares::zone::mono::state::on_operation_aborted() {
  server_.close_abruptly(session_.shared_from_this());
}

void ares::zone::mono::state::on_eof() {
  server_.close_abruptly(session_.shared_from_this());
}

void ares::zone::mono::state::on_socket_error() {
  server_.close_abruptly(session_.shared_from_this());
}

void ares::zone::mono::state::on_packet_processed() {
  // TODO: rotate obfuscation key
}

void ares::zone::mono::state::defuse_asio() {
}

auto ares::zone::mono::state::allocate(const uint16_t packet_id) -> packet::alloc_info {
  switch (packet_id) {
    ARES_ALLOCATE_PACKET_CASE(CZ_ENTER);
  default:
    { // Packet id is not known to this server under selected packet set
      log()->error("Unexpected packet_id {:#x} for mono session while allocating", packet_id);
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

void ares::zone::mono::state::dispatch_packet(const uint16_t packet_id, void* buf, std::function<void(void*)> deallocator) {
  switch (packet_id) {
    ARES_DISPATCH_PACKET_CASE(CZ_ENTER);
  default:
    {
      log()->error("Unexpected packet_id {:#x} for mono session while dispatching, disconnecting", packet_id);
      server_.close_gracefuly(session_.shared_from_this());
      session_.connected_ = false;
    }
  }
}

auto ares::zone::mono::state::log() const -> std::shared_ptr<spdlog::logger> {
  return server_.log();
}

auto ares::zone::mono::state::conf() const -> const config& {
  return server_.conf();
}
