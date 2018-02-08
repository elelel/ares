#include "state.hpp"
#include "../server.hpp"

void ares::account::character_server::packet_handler<ares::packet_set, ares::packet::ATHENA_HA_AID_AUTH_REQ>::operator()() {
  SPDLOG_TRACE(log(), "handle_packet ATHENA_HA_AID_AUTH_REQ: begin");

  using namespace rxcpp;
  server_.on_rxthreads
    ([this] () {
      auto found = server_.client_by_aid(p_->aid());
      if (found) {
        const auto& data = found->as_client();
        if ((data.auth_code1 == p_->auth_code1()) &&
            (data.auth_code2 == p_->auth_code2())) {
          session_.emplace_and_send<packet::type<packet_set, packet::ATHENA_AH_AID_AUTH_RESULT>>(p_->aid(),
                                                                                                 p_->auth_code1(),
                                                                                                 p_->auth_code2(),
                                                                                                 p_->sex(),
                                                                                                 0,
                                                                                                 p_->request_id(),
                                                                                                 data.version,
                                                                                                 data.clienttype);
        } else {
          log()->warn("AID {} authentication from char server failed, wrong auth codes", p_->aid());
          SPDLOG_TRACE(log(), "AID auth failure: data.auth_code1 = {}, p_->auth_code1 = {}, data.auth_code2 = {}, p_->auth_code2 = {}",
                       data.auth_code1,
                       p_->auth_code1(),
                       data.auth_code2,
                       p_->auth_code2());
          session_.emplace_and_send<packet::type<packet_set, packet::ATHENA_AH_AID_AUTH_RESULT>>(p_->aid(),
                                                                                                 p_->auth_code1(),
                                                                                                 p_->auth_code2(),
                                                                                                 p_->sex(),
                                                                                                 1,
                                                                                                 p_->request_id(),
                                                                                                 data.version,
                                                                                                 data.clienttype);
        }
      } else {
        log()->warn("Char server requested AID authentication for non-authenticated AID {}", p_->aid());
        session_.emplace_and_send<packet::type<packet_set, packet::ATHENA_AH_AID_AUTH_RESULT>>(p_->aid(),
                                                                                               p_->auth_code1(),
                                                                                               p_->auth_code2(),
                                                                                               p_->sex(),
                                                                                               1,
                                                                                               p_->request_id(),
                                                                                               0,
                                                                                               0);
      }
    });
  
  SPDLOG_TRACE(log(), "handle_packet ATHENA_HA_AID_AUTH_REQ: end");
}
