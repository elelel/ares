#include "state.hpp"

#include "../server.hpp"

void ares::zone::client::packet_handler<ares::packet::current<ares::packet::CZ_REQ_GUILD_MENU>>::operator()() {
  SPDLOG_TRACE(log(), "handle_packet CZ_REQ_GUILD_MENU: begin");
  // TODO: send guild info if applicable
  SPDLOG_TRACE(log(), "handle_packet CZ_REQ_GUILD_MENU: end");
}
