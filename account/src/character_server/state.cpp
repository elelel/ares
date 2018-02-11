#include "state.hpp"

#include "../server.hpp"
#include "../session.hpp"

ares::account::character_server::state::state(ares::account::server& serv,
                                           ares::account::session& sess) :
  server_(serv),
  session_(sess) {
  }

ares::account::character_server::state::state(const mono::state& mono_state) :
  server_(mono_state.server_),
  session_(mono_state.session_) {
}

void ares::account::character_server::state::on_connect() {
}

void ares::account::character_server::state::on_connection_reset() {
  log()->warn("Character server disconnected, closing it's session");
  // TODO: set offline aids
  server_.close_abruptly(session_.shared_from_this());
}

void ares::account::character_server::state::on_operation_aborted() {
  on_connection_reset();
}

void ares::account::character_server::state::on_eof() {
  on_connection_reset();
}

void ares::account::character_server::state::on_socket_error() {
  log()->warn("Character socket error, closing it's session");
  server_.close_abruptly(session_.shared_from_this());
}

void ares::account::character_server::state::on_packet_processed() {
}

void ares::account::character_server::state::defuse_asio() {
}

auto ares::account::character_server::state::allocate(const uint16_t packet_id) -> packet::alloc_info {
  switch (packet_id) {
    ARES_ALLOCATE_PACKET_CASE(ATHENA_HA_PING_REQ);
    ARES_ALLOCATE_PACKET_CASE(ATHENA_HA_ONLINE_AIDS);
    ARES_ALLOCATE_PACKET_CASE(ATHENA_HA_USER_COUNT);
    ARES_ALLOCATE_PACKET_CASE(ATHENA_HA_AID_AUTH_REQ);
    ARES_ALLOCATE_PACKET_CASE(ATHENA_HA_ACCOUNT_DATA_REQ);
    ARES_ALLOCATE_PACKET_CASE(ATHENA_HA_SET_AID_ONLINE);
    ARES_ALLOCATE_PACKET_CASE(ATHENA_HA_SET_AID_OFFLINE);
  default:
    { // Packet id is not known to this server under selected packet set
      log()->error("Unexpected packet_id {:#x} for character server session while allocating", packet_id);
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

void ares::account::character_server::state::dispatch_packet(const uint16_t packet_id, void* buf, std::function<void(void*)> deallocator) {
  switch (packet_id) {
    ARES_DISPATCH_PACKET_CASE(ATHENA_HA_PING_REQ);
    ARES_DISPATCH_PACKET_CASE(ATHENA_HA_ONLINE_AIDS);
    ARES_DISPATCH_PACKET_CASE(ATHENA_HA_USER_COUNT);
    ARES_DISPATCH_PACKET_CASE(ATHENA_HA_AID_AUTH_REQ);
    ARES_DISPATCH_PACKET_CASE(ATHENA_HA_ACCOUNT_DATA_REQ);
    ARES_DISPATCH_PACKET_CASE(ATHENA_HA_SET_AID_ONLINE);
    ARES_DISPATCH_PACKET_CASE(ATHENA_HA_SET_AID_OFFLINE);
  default:
    {
      log()->error("Unexpected packet_id {:#x} for mono session, disconnecting", packet_id);
      server_.close_gracefuly(session_.shared_from_this());
      session_.connected_ = false;
    }
  }
}

auto ares::account::character_server::state::log() const -> std::shared_ptr<spdlog::logger> {
  return server_.log();
}

auto ares::account::character_server::state::conf() const -> const config& {
  return server_.conf();
}
