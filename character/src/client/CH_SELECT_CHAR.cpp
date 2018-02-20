#include <ares/common>

#include "state.hpp"
#include "../server.hpp"

void ares::character::client::packet_handler<ares::packet::current<ares::packet::CH_SELECT_CHAR>>::operator()() {
  SPDLOG_TRACE(log(), "CH_SELECT_CHAR begin");
  auto& c = session_.as_client();
  if (p_->CharNum() < c.playable_slots) {
    auto ci = server_.db.character_info_for_slot(c.aid, p_->CharNum());
    if (ci) {
      std::lock_guard lock(server_.mutex());
      auto found_zs = server_.zone_server_by_map_id(ci->pc.location_last.map_id);
      if (found_zs != nullptr) {
        // Store selected char_info
        SPDLOG_TRACE(log(), "map server found, user selected cid {}", ci->pc.cid);
        if (!state_.char_info) {
          state_.char_info = std::move(ci);
        }
        SPDLOG_TRACE(log(), "sending NOTIFY_ZONESVR to client");
        const auto& map_name = server_.map_name_by_map_id(ci->pc.location_last.map_id);
        session_.emplace_and_send<packet::current<packet::HC_NOTIFY_ZONESVR>>(state_.char_info->pc.cid,
                                                                              map_name,
                                                                              htonl(found_zs->as_zone_server().ip_v4.to_ulong()),
                                                                              found_zs->as_zone_server().port);
      } else {
        log()->warn("AID {} requested map {} which is not found in any active zone servers", c.aid, ci->pc.location_last.map_id);
        session_.close_gracefuly();
      }
    } else {
      log()->warn("AID {} requested slot number {}, but char info is not found in database", c.aid, p_->CharNum());
      session_.close_gracefuly();
    }
  } else {
    log()->warn("AID {} requested slot number {} above playable slots {}", c.aid, p_->CharNum(), c.playable_slots);
    // TODO: Send slot is not usable
    session_.close_gracefuly();
  }
  SPDLOG_TRACE(log(), "CH_SELECT_CHAR end");
}
