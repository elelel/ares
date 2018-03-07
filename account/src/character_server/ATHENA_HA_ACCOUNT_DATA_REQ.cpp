#include "state.hpp"
#include "../server.hpp"

void ares::account::character_server::packet_handler<ares::packet::current<ares::packet::ATHENA_HA_ACCOUNT_DATA_REQ>>::operator()() {
  SPDLOG_TRACE(log(), "handle_packet ATHENA_HA_ACCOUNT_DATA_REQ: begin");
  auto ud = server_.db->query<database::accounts::user_data_for_id>(p_->aid());
  if (ud) {
    session_.emplace_and_send<packet::current<packet::ATHENA_AH_ACCOUNT_DATA_RESULT>>(ud->aid,
                                                                     ud->email,
                                                                     ud->expiration_time,
                                                                     ud->level,
                                                                     ud->birthdate,
                                                                     ud->pin);
  } else {
    log()->warn("Char server requested data for non-existent AID {}", p_->aid());
  }
  SPDLOG_TRACE(log(), "handle_packet ATHENA_HA_ACCOUNT_DATA_REQ: end");
}
