#include "state.hpp"
#include "../server.hpp"

void ares::account::character_server::packet_handler<ares::packet_set, ares::packet::ATHENA_HA_SET_AID_OFFLINE>::operator()() {
  SPDLOG_TRACE(log(), "handle_packet ATHENA_HA_SET_AID_OFFLINE: begin");

  using namespace rxcpp;
  server_.on_rxthreads([this] () {
      server_.unlink_aid_from_char_server(p_->aid(), session_.shared_from_this());
    });
  
  SPDLOG_TRACE(log(), "handle_packet ATHENA_HA_SET_AID_OFFLINE: end");
}
