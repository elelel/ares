#include "state.hpp"
#include "../server.hpp"

void ares::account::character_server::packet_handler<ares::packet_set, ares::packet::ATHENA_HA_PING_REQ>::operator()() {
  SPDLOG_TRACE(log(), "ATHENA_HA_PING_REQ begin");
  auto s = session_.shared_from_this();
  server_.on_rxthreads([this, s] () {
      if (server_.char_servers().find(session_.shared_from_this()) !=
          server_.char_servers().end()) {
        s->emplace_and_send<packet::type<packet_set, packet::ATHENA_AH_PING_ACK>>();
      } else {
        log()->warn("Char server packet ATHENA_HA_LOGIN_REQ received from session not listed as character server, closing.");
        server_.close_gracefuly(s);
      }
    });
  SPDLOG_TRACE(log(), "ATHENA_HA_PING_REQ end");
}
