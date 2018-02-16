#include "state.hpp"
#include "../server.hpp"

void ares::account::character_server::packet_handler<ares::packet::current<ares::packet::ATHENA_HA_PING_REQ>>::operator()() {
  SPDLOG_TRACE(log(), "ATHENA_HA_PING_REQ begin");
  auto s = session_.shared_from_this();
  std::lock_guard<std::mutex> lock(server_.mutex());
  if (server_.char_servers().find(session_.shared_from_this()) != server_.char_servers().end()) {
    s->emplace_and_send<packet::current<packet::ATHENA_AH_PING_ACK>>();
  } else {
    log()->warn("Char server packet ATHENA_HA_LOGIN_REQ received from session not listed as character server, closing.");
    session_.close_gracefuly();
  }

  SPDLOG_TRACE(log(), "ATHENA_HA_PING_REQ end");
}
