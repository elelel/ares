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

auto ares::character::mono::state::allocate(const uint16_t packet_id) -> packet::alloc_info {
  switch (packet_id) {
    ARES_ALLOCATE_PACKET_CASE(CH_ENTER);
    ARES_ALLOCATE_PACKET_CASE(ATHENA_ZH_LOGIN_REQ);
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

void ares::character::mono::state::dispatch_packet(void* buf, std::function<void(void*)> deallocator) {
  uint16_t* packet_id = reinterpret_cast<uint16_t*>(buf);
  switch (*packet_id) {
    ARES_DISPATCH_PACKET_CASE(CH_ENTER);
    ARES_DISPATCH_PACKET_CASE(ATHENA_ZH_LOGIN_REQ);
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
