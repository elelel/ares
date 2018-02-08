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
}

void ares::account::character_server::state::on_operation_aborted() {
}

void ares::account::character_server::state::on_eof() {
}

void ares::account::character_server::state::on_socket_error() {
}

void ares::account::character_server::state::on_packet_processed() {
}

size_t ares::account::character_server::state::dispatch_packet(const uint16_t packet_id) {
  switch (packet_id) {
    ARES_PACKET_CASE(ATHENA_HA_PING_REQ);
    ARES_PACKET_CASE(ATHENA_HA_ONLINE_AIDS);
    ARES_PACKET_CASE(ATHENA_HA_USER_COUNT);
    ARES_PACKET_CASE(ATHENA_HA_AID_AUTH_REQ);
    ARES_PACKET_CASE(ATHENA_HA_ACCOUNT_DATA_REQ);
    ARES_PACKET_CASE(ATHENA_HA_SET_AID_ONLINE);
    ARES_PACKET_CASE(ATHENA_HA_SET_AID_OFFLINE);
  default:
    {
      log()->error("Unexpected packet_id {:#x} for mono::state session, disconnecting", packet_id);
      server_.close_gracefuly(session_.shared_from_this());
      session_.connected_ = false;
      return 0;
    }
  }
}

auto ares::account::character_server::state::log() const -> std::shared_ptr<spdlog::logger> {
  return server_.log();
}

auto ares::account::character_server::state::conf() const -> const config& {
  return server_.conf();
}
