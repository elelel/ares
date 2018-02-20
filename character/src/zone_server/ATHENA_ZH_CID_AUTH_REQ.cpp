#include "state.hpp"
#include "../server.hpp"

void ares::character::zone_server::packet_handler<ares::packet::current<ares::packet::ATHENA_ZH_CID_AUTH_REQ>>::operator()() {
  SPDLOG_TRACE(log(), "ATHENA_ZH_CID_AUTH_REQ: begin");
  session_ptr found;
  {
    std::lock_guard<std::mutex> lock(server_.mutex());
    found = server_.client_by_aid(p_->aid());
  }
  if (found) {
    const auto& data = found->as_client();
    if ((data.auth_code1 == p_->auth_code1()) && (data.aid == p_->aid())) {
      if (data.char_info) {
        if ((data.char_info->pc.sex == p_->sex()) && (data.char_info->pc.cid == p_->cid())) {
          SPDLOG_TRACE(log(), "Authenticated aid {}, cid{} to zone (char name '{}', map id '{}')",
                       p_->aid(), p_->cid(), (const char*)data.char_info->pc.name, data.char_info->pc.location_last.map_id);
          session_.emplace_and_send<packet::current<packet::ARES_HZ_CID_AUTH_OK>>(data.aid,
                                                                                  data.auth_code1,
                                                                                  data.auth_code2,
                                                                                  0,
                                                                                  0,
                                                                                  0,
                                                                                  data.char_info->pc);

        } else {
          log()->warn("Failed authentication attempt from zone active aid {} with cid {}, wrong sex", p_->aid(), p_->cid());
          session_.emplace_and_send<packet::current<packet::ATHENA_HZ_CID_AUTH_FAILED>>(p_->aid(),
                                                                                        p_->cid(),
                                                                                        p_->auth_code1(),
                                                                                        p_->sex(),
                                                                                        0);
        }
      } else {
        log()->error("Char info is not loaded from database while authenticating aid {} from zone with cid {}, auth_code1 {:#x}", p_->aid(), p_->cid(), p_->auth_code1());
        session_.emplace_and_send<packet::current<packet::ATHENA_HZ_CID_AUTH_FAILED>>(p_->aid(),
                                                                                      p_->cid(),
                                                                                      p_->auth_code1(),
                                                                                      p_->sex(),
                                                                                      0);
      }
    } else {
      log()->warn("Failed authentication attempt from zone for active aid. Packet/data: aid {}/{}, cid {}/{}, auth_code1 {:#x}/{:#x}",
                  p_->aid(), data.aid, p_->cid(), data.char_info->pc.cid, p_->auth_code1(), data.auth_code1);
      session_.emplace_and_send<packet::current<packet::ATHENA_HZ_CID_AUTH_FAILED>>(p_->aid(),
                                                                                    p_->cid(),
                                                                                    p_->auth_code1(),
                                                                                    p_->sex(),
                                                                                    0);
    }
  } else {
    log()->warn("Failed authentication attempt from zone for inactive aid {} with cid {}, auth_code1 {:#x}", p_->aid(), p_->cid(), p_->auth_code1());
    session_.emplace_and_send<packet::current<packet::ATHENA_HZ_CID_AUTH_FAILED>>(p_->aid(),
                                                                                  p_->cid(),
                                                                                  p_->auth_code1(),
                                                                                  p_->sex(),
                                                                                  0);
  }
  SPDLOG_TRACE(log(), "ATHENA_ZH_CID_AUTH_REQ: end");
}
