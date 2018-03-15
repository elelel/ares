#include "state.hpp"
#include "../server.hpp"

void ares::account::character_server::packet_handler<ares::packet::current<ares::packet::ARES_HA_PING_REQ>>::operator()() {
  SPDLOG_TRACE(log(), "ARES_HA_PING_REQ begin");
  auto s = session_.shared_from_this();
  std::lock_guard<std::mutex> lock(server_.mutex());
  s->emplace_and_send<packet::current<packet::ARES_AH_PING_ACK>>();
  SPDLOG_TRACE(log(), "ARES_HA_PING_REQ end");
}
