#include "state.hpp"

#include <ares/packets>

#include "../server.hpp"
#include "../session.hpp"

ares::zone::character_server::state::state(ares::zone::server& serv,
                                           ares::zone::session& sess) :
  server_(serv),
  session_(sess) {
  }

void ares::zone::character_server::state::on_connect() {
  log()->info("Starting handshake with character server");
  const auto my_ipv4 = conf().listen_ipv4[0].address().to_v4().to_ulong();
  const auto my_port = conf().listen_ipv4[0].port();
  session_.emplace_and_send<packet::type<packet_set, packet::ATHENA_ZH_LOGIN_REQ>>(conf().character_server->login,
                                                                                   conf().character_server->password,
                                                                                   0,
                                                                                   htonl(my_ipv4),
                                                                                   htons(my_port));
}

void ares::zone::character_server::state::on_connection_reset() {
  log()->warn("Disconnected from character server, reconnecting");
  session_.set_reconnect_timer(std::chrono::seconds{0}, std::chrono::seconds{5});
}

void ares::zone::character_server::state::on_operation_aborted() {
}

void ares::zone::character_server::state::on_eof() {
  on_connection_reset();
}

void ares::zone::character_server::state::on_socket_error() {
}

void ares::zone::character_server::state::on_packet_processed() {
}

size_t ares::zone::character_server::state::dispatch_packet(const uint16_t packet_id) {
  switch (packet_id) {
    ARES_PACKET_CASE(ATHENA_HZ_LOGIN_RESULT);
    ARES_PACKET_CASE(ATHENA_HZ_PRIVATE_MSG_NAME);
    ARES_PACKET_CASE(ATHENA_HZ_PING_ACK);
    ARES_PACKET_CASE(ARES_HZ_MAP_NAMES);
  default:
    {
      log()->error("Unexpected packet_id {:#x} for character_server::state session, disconnecting", packet_id);
      server_.close_gracefuly(session_.shared_from_this());
      session_.connected_ = false;
      return 0;
    }
  }
}

auto ares::zone::character_server::state::log() const -> std::shared_ptr<spdlog::logger> {
  return server_.log();
}

auto ares::zone::character_server::state::conf() const -> const config& {
  return server_.conf();
}
