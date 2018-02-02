#include "state.hpp"
#include "../state.hpp"

void ares::account::char_server::packet_handler<ares::packet_set, ares::packet::ATHENA_HA_PING_REQ>::operator()() {
  SPDLOG_TRACE(log(), "ATHENA_HA_PING_REQ begin");
  if (server_state_.server.char_servers().find(session_.shared_from_this()) !=
      server_state_.server.char_servers().end()) {
    session_.emplace_and_send<packet_type<packet::ATHENA_AH_PING_ACK>>();
  } else {
    log()->warn("Char server packet ATHENA_HA_LOGIN_REQ received from clients not listed in server's character server list, closing.");
    throw network::terminate_session();
  }
  SPDLOG_TRACE(log(), "ATHENA_HA_PING_REQ end");
}
