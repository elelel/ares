#include "state.hpp"

#include "../server.hpp"

void ares::zone::client::packet_handler<ares::packet::current<ares::packet::CZ_REQ_SCHEDULER_CASHITEM>>::operator()() {
  SPDLOG_TRACE(log(), "handle_packet CZ_REQ_SCHEDULER_CASHITEM: begin");
  // TODO: ZC_ACK_SCHDULER_CASHITEM if applicable
  SPDLOG_TRACE(log(), "handle_packet CZ_REQ_SCHEDULER_CASHITEM: end");
}
