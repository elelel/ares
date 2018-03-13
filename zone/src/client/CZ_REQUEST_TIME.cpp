#include "state.hpp"

#include "../server.hpp"

void ares::zone::client::packet_handler<ares::packet::current<ares::packet::CZ_REQUEST_TIME>>::operator()() {
  SPDLOG_TRACE(log(), "handle_packet CZ_REQUEST_TIME: begin");
  session_.as_client().tick = p_->clientTime();
  session_.emplace_and_send<packet::current<packet::ZC_NOTIFY_TIME>>();
  SPDLOG_TRACE(log(), "handle_packet CZ_REQUEST_TIME: end");
}
