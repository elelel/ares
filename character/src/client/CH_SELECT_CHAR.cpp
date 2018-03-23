#include "state.hpp"
#include "../server.hpp"

void ares::character::client::packet_handler<ares::packet::current<ares::packet::CH_SELECT_CHAR>>::operator()() {
  SPDLOG_TRACE(log(), "CH_SELECT_CHAR begin");
  auto& c = session_.as_client();
  if (p_->CharNum() < c.playable_slots) {
    auto ci = server_.db->query<database::characters::info_for_slot>(c.account_id, p_->CharNum());
    if (ci) {
      std::lock_guard<std::mutex> lock(server_.mutex());
      auto found_zs = server_.maps->zone_session_by_id(ci->location_last.map_id);
      if (found_zs != nullptr) {
        // Store selected char_info
        SPDLOG_TRACE(log(), "map server found, user selected character_id {}", ci->character_id.to_string());
        if (!state_.char_info) {
          state_.char_info = std::move(ci);
        }
        const auto& map_name = server_.maps->name_by_id(ci->location_last.map_id);
        SPDLOG_TRACE(log(), "Sending NOTIFY_ZONESVR to client. GID {}, character_id {}, map_name {}, ip {}, port {}",
                     model::GID(state_.char_info->character_id).to_string(), state_.char_info->character_id.to_string(),
                     map_name, found_zs->as_zone_server().ip_v4.to_string(),  found_zs->as_zone_server().port);
        session_.emplace_and_send<packet::current<packet::HC_NOTIFY_ZONESVR>>(model::GID(state_.char_info->character_id),
                                                                              map_name,
                                                                              htonl(found_zs->as_zone_server().ip_v4.to_ulong()),
                                                                              found_zs->as_zone_server().port);
      } else {
        log()->warn("Account id {} requested map {} which is not found in any active zone servers", c.account_id.to_string(), ci->location_last.map_id);
        session_.close_gracefuly();
      }
    } else {
      log()->warn("Account id {} requested slot number {}, but char info is not found in database", c.account_id.to_string(), p_->CharNum());
      session_.close_gracefuly();
    }
  } else {
    log()->warn("Account id {} requested slot number {} above playable slots {}", c.account_id.to_string(), p_->CharNum(), c.playable_slots);
    // TODO: Send slot is not usable
    session_.close_gracefuly();
  }
  SPDLOG_TRACE(log(), "CH_SELECT_CHAR end");
}
