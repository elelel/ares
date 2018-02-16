#include "state.hpp"

#include <ares/packets>
#include <ares/network>

#include "../server.hpp"

ares::character::zone_server::state::state(character::server& serv, session& sess) :
  server_(serv),
  session_(sess) {
  }

ares::character::zone_server::state::state(const mono::state& mono_state) :
  server_(mono_state.server_),
  session_(mono_state.session_) {
}

void ares::character::zone_server::state::on_connect() {
}

void ares::character::zone_server::state::on_connection_reset() {
  session_.close_abruptly();
}

void ares::character::zone_server::state::on_operation_aborted() {
  session_.close_abruptly();
}

void ares::character::zone_server::state::on_eof() {
  session_.close_abruptly();
}

void ares::character::zone_server::state::on_socket_error() {
  session_.close_abruptly();
}

void ares::character::zone_server::state::defuse_asio() {
}

auto ares::character::zone_server::state::allocate(const uint16_t packet_id) -> packet::alloc_info {
  switch (packet_id) {
    ARES_ALLOCATE_PACKET_CASE(ATHENA_ZH_ONLINE_USERS);
    ARES_ALLOCATE_PACKET_CASE(ATHENA_ZH_USER_COUNT);
    ARES_ALLOCATE_PACKET_CASE(ATHENA_ZH_GAME_RATES);
    ARES_ALLOCATE_PACKET_CASE(ATHENA_ZH_PING_REQ);
    ARES_ALLOCATE_PACKET_CASE(ARES_ZH_MAP_NAMES_REQ);
    ARES_ALLOCATE_PACKET_CASE(ATHENA_ZH_CID_AUTH_REQ);
  default:
    { // Packet id is not known to this server under selected packet set
      log()->error("Unexpected packet_id {:#x} for zone server session while allocating", packet_id);
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

void ares::character::zone_server::state::dispatch_packet(void* buf, std::function<void(void*)> deallocator) {
  uint16_t* packet_id = reinterpret_cast<uint16_t*>(buf);
  switch (*packet_id) {
    ARES_DISPATCH_PACKET_CASE(ATHENA_ZH_ONLINE_USERS);
    ARES_DISPATCH_PACKET_CASE(ATHENA_ZH_USER_COUNT);
    ARES_DISPATCH_PACKET_CASE(ATHENA_ZH_GAME_RATES);
    ARES_DISPATCH_PACKET_CASE(ATHENA_ZH_PING_REQ);
    ARES_DISPATCH_PACKET_CASE(ARES_ZH_MAP_NAMES_REQ);
    ARES_DISPATCH_PACKET_CASE(ATHENA_ZH_CID_AUTH_REQ);
  default:
    {
      log()->error("Unexpected packet_id {:#x} for zone server session while dispatching, disconnecting", *packet_id);
      session_.close_gracefuly();
      session_.connected_ = false;
    }
  }
}

auto ares::character::zone_server::state::log() const -> std::shared_ptr<spdlog::logger> {
  return server_.log();
}

auto ares::character::zone_server::state::conf() const -> const config& {
  return server_.conf();
}
