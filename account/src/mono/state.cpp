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
}

void ares::account::mono::state::on_operation_aborted() {
}

void ares::account::mono::state::on_eof() {
}

void ares::account::mono::state::on_socket_error() {
}

void ares::account::mono::state::on_packet_processed() {
}

size_t ares::account::mono::state::dispatch_packet(const uint16_t packet_id) {
  switch (packet_id) {
    ARES_PACKET_CASE(CA_EXE_HASHCHECK);
    // ARES_PACKET_CASE(CA_SSO_LOGIN_REQ::login_password);
    // ARES_PACKET_CASE(CA_SSO_LOGIN_REQ::token_auth);
    ARES_PACKET_CASE(ATHENA_HA_LOGIN_REQ);
    ARES_PACKET_CASE(ATHENA_HA_PING_REQ);
  default:
    {
      log()->error("Unexpected packet_id {:#x} for mono::state session, disconnecting", packet_id);
      server_.close_gracefuly(session_.shared_from_this());
      session_.connected_ = false;
      return 0;
    }
  }
}

auto ares::account::mono::state::log() const -> std::shared_ptr<spdlog::logger> {
  return server_.log();
}

auto ares::account::mono::state::conf() const -> const config& {
  return server_.conf();
}
