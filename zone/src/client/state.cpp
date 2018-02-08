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
}

void ares::zone::client::state::on_operation_aborted() {
}

void ares::zone::client::state::on_eof() {
}

void ares::zone::client::state::on_socket_error() {
}

void ares::zone::client::state::on_packet_processed() {
}

size_t ares::zone::client::state::dispatch_packet(const uint16_t packet_id) {
  switch (packet_id) {
  default:
    {
      log()->error("Unexpected packet_id {:#x} for client::state session, disconnecting", packet_id);
      server_.close_gracefuly(session_.shared_from_this());
      session_.connected_ = false;
      return 0;
    }
  }
}

auto ares::zone::client::state::log() const -> std::shared_ptr<spdlog::logger> {
  return server_.log();
}

auto ares::zone::client::state::conf() const -> const config& {
  return server_.conf();
}
