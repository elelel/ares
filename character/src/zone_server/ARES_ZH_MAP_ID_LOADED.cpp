#include "state.hpp"
#include "../server.hpp"

void ares::character::zone_server::packet_handler<ares::packet::current<ares::packet::ARES_ZH_MAP_ID_LOADED>>::operator()() {
  SPDLOG_TRACE(log(), "ARES_ZH_MAP_ID_LOADED: begin");
  /*  std::lock_guard<std::mutex> lock(server_.mutex());
  if (!server_.maps->link_id_to_zone_session(p_->map_id(), session_.shared_from_this())) {
    log()->error("Failed assigning map id {} to the zone server, closing session", p_->map_id());
    session_.close_gracefuly();
  }

  // Remove?
  session_.as_zone_server().map_ids.insert(p_->map_id()); */

  SPDLOG_TRACE(log(), "ARES_ZH_MAP_ID_LOADED: end");
}
