#include "state.hpp"
#include "../server.hpp"

void ares::character::zone_server::packet_handler<ares::packet::current<ares::packet::ATHENA_ZH_ONLINE_USERS>>::operator()() {
  SPDLOG_TRACE(log(), "ATHENA_HA_ONLINE_USERS: begin");
  const size_t user_count = (p_->PacketLength - sizeof(*p_)) / sizeof(handled_packet_type::aid_cid);
  SPDLOG_TRACE(log(), "Got {} users from map", user_count);
  auto& online_aid_to_cid = session_.as_zone_server().online_aid_to_cid;
  auto& online_cid_to_aid = session_.as_zone_server().online_cid_to_aid;
  online_aid_to_cid.clear();
  online_cid_to_aid.clear();
  for (size_t i = 0; i < user_count; ++i) {
    online_aid_to_cid.insert({p_->users()[i].aid, p_->users()[i].cid});
    online_cid_to_aid.insert({p_->users()[i].cid, p_->users()[i].aid});
  }
  SPDLOG_TRACE(log(), "ATHENA_HA_ONLINE_USERS: end");
}
