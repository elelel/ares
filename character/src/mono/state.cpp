#include "state.hpp"

#include <ares/packets>
#include <ares/network>

#include "../server.hpp"

ares::character::mono::state::state(character::server& serv, session& sess) :
  server_(serv),
  session_(sess) {
  }


void ares::character::mono::state::on_connect() {
}

void ares::character::mono::state::on_connection_reset() {
  server_.close_abruptly(session_.shared_from_this());
}

void ares::character::mono::state::on_operation_aborted() {
  server_.close_abruptly(session_.shared_from_this());
}

void ares::character::mono::state::on_eof() {
  server_.close_abruptly(session_.shared_from_this());
}

void ares::character::mono::state::on_socket_error() {
  server_.close_abruptly(session_.shared_from_this());
}

void ares::character::mono::state::on_packet_processed() {
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

void ares::character::mono::state::dispatch_packet(const uint16_t packet_id, void* buf, std::function<void(void*)> deallocator) {
  SPDLOG_TRACE(log(), "account_server::state::dispatch() switching on PacketType = {0:#x}", packet_id);
  switch (packet_id) {
    ARES_DISPATCH_PACKET_CASE(CH_ENTER);
    ARES_DISPATCH_PACKET_CASE(ATHENA_ZH_LOGIN_REQ);
  default:
    {
      log()->error("Unexpected packet_id {:#x} for mono session while dispatching, disconnecting", packet_id);
      server_.close_gracefuly(session_.shared_from_this());
      session_.connected_ = false;
    }
  }
}

auto ares::character::mono::state::log() const -> std::shared_ptr<spdlog::logger> {
  return server_.log();
}

auto ares::character::mono::state::conf() const -> const config& {
  return server_.conf();
}
