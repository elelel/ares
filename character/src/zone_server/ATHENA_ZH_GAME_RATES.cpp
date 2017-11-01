#include "packet_handlers.hpp"

#include "state.hpp"
#include "../server.hpp"

void ares::character::zone_server::packet_handler<ares::packet::ATHENA_ZH_GAME_RATES>::operator()() {
  SPDLOG_TRACE(log(), "ATHENA_HA_GAME_RATES: begin");
  auto& s = session_.as_zone_server();
  s.base_rate = p_->base_rate();
  s.job_rate = p_->job_rate();
  s.drop_rate = p_->drop_rate();
  SPDLOG_TRACE(log(), "ATHENA_HA_GAME_RATES: end");
}
