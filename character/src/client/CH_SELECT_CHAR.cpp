#include "packet_handlers.hpp"

#include <ares/common>

#include "state.hpp"
#include "../server.hpp"

void ares::character::client::packet_handler<ares::packet::CH_SELECT_CHAR>::operator()() {
  SPDLOG_TRACE(log(), "CH_SELECT_CHAR begin");
  auto& c = session_.as_client();
  if (p_->CharNum() < c.playable_slots) {
    auto ci = server_.db().character_info_for_slot(c.cid, p_->CharNum());
    if (ci) {
      // TODO: Find zone server with last map and send zone server info
    } else {
      log()->warn("AID {} requested slot number {}, but char info is not found in database", c.aid, p_->CharNum());
      {
        std::lock_guard<std::mutex> lock(server_.mutex());
        server_.remove(session_.shared_from_this());
      }
    }
  } else {
    log()->warn("AID {} requested slot number {} above playable slots {}", c.aid, p_->CharNum(), c.playable_slots);
    // TODO: Send slot is not usable
    {
      std::lock_guard<std::mutex> lock(server_.mutex());
      server_.remove(session_.shared_from_this());
    }
  }
  SPDLOG_TRACE(log(), "CH_SELECT_CHAR end");
}
