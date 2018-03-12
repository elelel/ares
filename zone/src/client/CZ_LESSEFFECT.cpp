#include "state.hpp"

#include "../server.hpp"

void ares::zone::client::packet_handler<ares::packet::current<ares::packet::CZ_LESSEFFECT>>::operator()() {
  SPDLOG_TRACE(log(), "handle_packet CZ_LESSEFFECT: begin");

  SPDLOG_TRACE(log(), "handle_packet CZ_LESSEFFECT: end");
}
