#include "packet_handlers.hpp"

#include "state.hpp"
#include "../server.hpp"

void ares::account::char_server::packet_handler<ares::packet::ATHENA_HA_PING_REQ>::operator()() {
  SPDLOG_TRACE(log(), "ATHENA_HA_PING_REQ begin");
  if (server_.char_servers().find(session_.shared_from_this()) != server_.char_servers().end()) {
    session_.emplace_and_send<packet::ATHENA_AH_PING_ACK>();
  } else {
    log()->warn("Char server packet ATHENA_HA_LOGIN_REQ received from clients not listed in server's character server list, closing.");
    throw network::terminate_session();
  }
  SPDLOG_TRACE(log(), "ATHENA_HA_PING_REQ end");
}
