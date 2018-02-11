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
}

void ares::account::client::state::on_connection_reset() {
}

void ares::account::client::state::on_operation_aborted() {
}

void ares::account::client::state::on_eof() {
}

void ares::account::client::state::on_socket_error() {
}

void ares::account::client::state::on_packet_processed() {
}

void ares::account::client::state::defuse_asio() {
}

auto ares::account::client::state::allocate(const uint16_t packet_id) -> packet::alloc_info {
  switch (packet_id) {
  default:
    { // Packet id is not known to this server under selected packet set
      log()->error("Unexpected packet_id {:#x} for client::state session while allocating", packet_id);
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

void ares::account::client::state::dispatch_packet(const uint16_t packet_id, void* /*buf*/, std::function<void(void*)> /*deallocator*/) {
  switch (packet_id) {
  default:
    {
      log()->error("Unexpected packet_id {:#x} for mono::state session while dispatching, disconnecting", packet_id);
      server_.close_gracefuly(session_.shared_from_this());
      session_.connected_ = false;
    }
  }
}

auto ares::account::client::state::log() const -> std::shared_ptr<spdlog::logger> {
  return server_.log();
}

auto ares::account::client::state::conf() const -> const config& {
  return server_.conf();
}
