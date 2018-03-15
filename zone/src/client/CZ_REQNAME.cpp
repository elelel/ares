#include "state.hpp"

#include "../server.hpp"

void ares::zone::client::packet_handler<ares::packet::current<ares::packet::CZ_REQNAME>>::operator()() {
  SPDLOG_TRACE(log(), "handle_packet CZ_REQNAME: begin");
  // TODO: send name corresponding to the id
  SPDLOG_TRACE(log(), "CZ_REQNAME: requested name for AID {} ", p_->AID());
  SPDLOG_TRACE(log(), "handle_packet CZ_REQNAME: end");
}
