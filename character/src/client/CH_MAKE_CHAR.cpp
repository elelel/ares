#include "packet_handlers.hpp"

#include "state.hpp"
#include "../server.hpp"

void ares::character::client::packet_handler<ares::packet::CH_MAKE_CHAR>::operator()() {
  SPDLOG_TRACE(log(), "handle_packet CH_MAKE_CHAR begin");
  
  SPDLOG_TRACE(log(), "handle_packet CH_MAKE_CHAR end");
}
