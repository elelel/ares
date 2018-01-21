#include "packet_handlers.hpp"

#include "state.hpp"
#include "../server.hpp"

void ares::character::client::packet_handler<ares::packet::PING>::operator()() {
  SPDLOG_TRACE(log(), "handle_packet PING from client, aid {} ", p_.aid());
}

