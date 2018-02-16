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
      SPDLOG_TRACE(log(), "Searching for map {} in {} zone server session(s)", ci->location.map_name, server_.zone_servers().size());
      auto found_zs = std::find_if(server_.zone_servers().begin(), server_.zone_servers().end(),
                                   [this, &ci] (const session_ptr& s) {
                                     auto state = s->as_zone_server();
                                     if (state.map_names.find(ci->location.map_name) != state.map_names.end()) return true;
                                     SPDLOG_TRACE(log(), "searching for map name in zone server's map_names (total {})", state.map_names.size());
                                     return false;
                                   });
      if (found_zs != server_.zone_servers().end()) {
        // Store selected char_info
        if (!state_.char_info) { state_.char_info = std::move(ci); };
        SPDLOG_TRACE(log(), "map server found, sendint NOTIFY_ZONESVR to client");
        session_.emplace_and_send<packet::current<packet::HC_NOTIFY_ZONESVR>>(ci->info.cid,
                                                                              ci->location.map_name + ".gat",
                                                                              htonl((*found_zs)->as_zone_server().ip_v4.to_ulong()),
                                                                              (*found_zs)->as_zone_server().port);
      } else {
        log()->warn("AID {} requested map {} which is not found in any active zone servers", c.aid, ci->location.map_name);
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
