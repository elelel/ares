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
}

void ares::zone::mono::state::on_operation_aborted() {
}

void ares::zone::mono::state::on_eof() {
}

void ares::zone::mono::state::on_socket_error() {
}

void ares::zone::mono::state::on_packet_processed() {
}

size_t ares::zone::mono::state::dispatch_packet(const uint16_t packet_id) {
  switch (packet_id) {
    ARES_PACKET_CASE(CZ_ENTER);
  default:
    {
      log()->error("Unexpected packet_id {:#x} for mono::state session, disconnecting", packet_id);
      server_.close_gracefuly(session_.shared_from_this());
      session_.connected_ = false;
      return 0;
    }
  }
}

auto ares::zone::mono::state::log() const -> std::shared_ptr<spdlog::logger> {
  return server_.log();
}

auto ares::zone::mono::state::conf() const -> const config& {
  return server_.conf();
}
