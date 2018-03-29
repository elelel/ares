#include "state.hpp"

#include "../server.hpp"
#include "../session.hpp"

ares::account::client::state::state(ares::account::server& serv,
                                    ares::account::session& sess) :
  server_(serv),
  session_(sess) {
  }

ares::account::client::state::state(const mono::state& mono_state) :
  server_(mono_state.server_),
  session_(mono_state.session_),
  client_exe_hash(mono_state.client_exe_hash) {
}

void ares::account::client::state::on_connect() {
  session_.close_abruptly();
}

void ares::account::client::state::on_connection_reset() {
  session_.close_abruptly();
}

void ares::account::client::state::on_operation_aborted() {
  session_.close_abruptly();
}

void ares::account::client::state::on_eof() {
}

void ares::account::client::state::on_socket_error() {
}

void ares::account::client::state::defuse_asio() {
}

auto ares::account::client::state::packet_sizes(const uint16_t packet_id) -> std::tuple<size_t, size_t, size_t> {
  switch (packet_id) {
  default:
    {
      log()->error("Unexpected packet_id {:#x} for client session while getting packet sizes", packet_id);
      return std::tuple<size_t, size_t, size_t>(0, 0, 0);
    }
  }
}

void ares::account::client::state::dispatch_packet(std::shared_ptr<std::byte[]> buf) {
  uint16_t* packet_id = reinterpret_cast<uint16_t*>(buf.get());
  switch (*packet_id) {
  default:
    {
      log()->error("Unexpected packet_id {:#x} for client::state session while dispatching, disconnecting", *packet_id);
      session_.close_gracefuly();
      return;
    }
  }
}

auto ares::account::client::state::log() const -> std::shared_ptr<spdlog::logger> {
  return server_.log();
}

auto ares::account::client::state::conf() const -> const config& {
  return server_.conf();
}
