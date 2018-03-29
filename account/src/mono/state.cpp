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
  session_.close_abruptly();
}

void ares::account::mono::state::on_operation_aborted() {
  session_.close_abruptly();
}

void ares::account::mono::state::on_eof() {
  // This can be correct disconnect by client, do not close the authenticated session for now
  // TODO: make authenticated AIDs independent from sessions 
}

void ares::account::mono::state::on_socket_error() {
  session_.close_abruptly();
}

void ares::account::mono::state::defuse_asio() {
}

auto ares::account::mono::state::packet_sizes(const uint16_t packet_id) -> std::tuple<size_t, size_t, size_t> {
  switch (packet_id) {
    ARES_PACKET_SIZES_CASE(CA_EXE_HASHCHECK);
    ARES_PACKET_SIZES_CASE(CA_SSO_LOGIN_REQ::login_password);
    ARES_PACKET_SIZES_CASE(CA_SSO_LOGIN_REQ::token_auth);
    ARES_PACKET_SIZES_CASE(ARES_HA_LOGIN_REQ);
  default:
    {
      log()->error("Unexpected packet_id {:#x} for mono session while getting packet sizes", packet_id);
      return std::tuple<size_t, size_t, size_t>(0, 0, 0);
    }
  }
}

void ares::account::mono::state::dispatch_packet(std::shared_ptr<std::byte[]> buf) {
  uint16_t* packet_id = reinterpret_cast<uint16_t*>(buf.get());
  switch (*packet_id) {
    ARES_DISPATCH_PACKET_CASE(CA_EXE_HASHCHECK);
    ARES_DISPATCH_PACKET_CASE(CA_SSO_LOGIN_REQ::login_password);
    ARES_DISPATCH_PACKET_CASE(CA_SSO_LOGIN_REQ::token_auth);
    ARES_DISPATCH_PACKET_CASE(ARES_HA_LOGIN_REQ);
  default:
    {
      log()->error("Unexpected packet_id {:#x} for mono session while dispatching, disconnecting", *packet_id);
      session_.close_gracefuly();
      return;
    }
  }
}

auto ares::account::mono::state::log() const -> std::shared_ptr<spdlog::logger> {
  return server_.log();
}

auto ares::account::mono::state::conf() const -> const config& {
  return server_.conf();
}
