#include "state.hpp"

#include "../server.hpp"
#include "../session.hpp"

ares::account::mono::state::state(ares::account::server& serv,
                                           ares::account::session& sess) :
  server_(serv),
  session_(sess) {
  }

void ares::account::mono::state::on_connect() {
}

void ares::account::mono::state::on_connection_reset() {
  server_.close_abruptly(session_.shared_from_this());
}

void ares::account::mono::state::on_operation_aborted() {
  server_.close_abruptly(session_.shared_from_this());
}

void ares::account::mono::state::on_eof() {
  // This can be correct disconnect by client, do not close the authenticated session
}

void ares::account::mono::state::on_socket_error() {
  server_.close_abruptly(session_.shared_from_this());
}

void ares::account::mono::state::on_packet_processed() {
}

void ares::account::mono::state::defuse_asio() {
}

auto ares::account::mono::state::allocate(const uint16_t packet_id) -> packet::alloc_info {
  switch (packet_id) {
    ARES_ALLOCATE_PACKET_CASE(CA_EXE_HASHCHECK);
    ARES_ALLOCATE_PACKET_CASE(CA_SSO_LOGIN_REQ::login_password);
    ARES_ALLOCATE_PACKET_CASE(CA_SSO_LOGIN_REQ::token_auth);
    ARES_ALLOCATE_PACKET_CASE(ATHENA_HA_LOGIN_REQ);
    ARES_ALLOCATE_PACKET_CASE(ATHENA_HA_PING_REQ);
  default:
    { // Packet id is not known to this server under selected packet set
      log()->error("Unexpected packet_id {:#x} for mono::state session while allocating", packet_id);
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

void ares::account::mono::state::dispatch_packet(const uint16_t packet_id, void* buf, std::function<void(void*)> deallocator) {
  switch (packet_id) {
    ARES_DISPATCH_PACKET_CASE(CA_EXE_HASHCHECK);
    ARES_DISPATCH_PACKET_CASE(CA_SSO_LOGIN_REQ::login_password);
    ARES_DISPATCH_PACKET_CASE(CA_SSO_LOGIN_REQ::token_auth);
    ARES_DISPATCH_PACKET_CASE(ATHENA_HA_LOGIN_REQ);
    ARES_DISPATCH_PACKET_CASE(ATHENA_HA_PING_REQ);
  default:
    {
      log()->error("Unexpected packet_id {:#x} for mono::state session while dispatching, disconnecting", packet_id);
      server_.close_gracefuly(session_.shared_from_this());
      session_.connected_ = false;
    }
  }
}

auto ares::account::mono::state::log() const -> std::shared_ptr<spdlog::logger> {
  return server_.log();
}

auto ares::account::mono::state::conf() const -> const config& {
  return server_.conf();
}
