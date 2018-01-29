#include "state.hpp"
#include "../state.hpp"

void ares::character::zone_server::packet_handler<ares::packet::ATHENA_ZH_GAME_RATES>::operator()() {
  SPDLOG_TRACE(server_state_.log(), "ATHENA_HA_GAME_RATES: begin");
  auto& s = session_.as_zone_server();
  s.base_rate = p_->base_rate();
  s.job_rate = p_->job_rate();
  s.drop_rate = p_->drop_rate();
  SPDLOG_TRACE(server_state_.log(), "ATHENA_HA_GAME_RATES: end");
}
