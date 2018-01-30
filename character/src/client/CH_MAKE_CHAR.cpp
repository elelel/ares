#include <ares/common>

#include "state.hpp"
#include "../state.hpp"

void ares::character::client::packet_handler<ares::packet::CH_MAKE_CHAR>::operator()() {
  SPDLOG_TRACE(log(), "CH_MAKE_CHAR begin");
  auto& c = session_.as_client();
  auto& db = server_state_.db;
  auto slots = db.account_slots_for_aid(c.aid);
  if (slots) {
    // TODO: Check if new char creation is allowed in server configuration
    if (p_->CharNum() < slots->creatable_slots) {
      // TODO: Check if slot is not used
      const auto job = static_cast<ares::JOB>(p_->job());
      if ((job == JOB::NOVICE) || (job == JOB::SUMMONER)) {
        // TODO: Starting zeny, locations from server config
        const uint32_t starting_zeny = 1000;
        std::string starting_map{"prt_fild00"};
        uint16_t starting_map_x{150};
        uint16_t starting_map_y{150};
        if (job == JOB::SUMMONER) {
          starting_map = "lasa_fild01";
          starting_map_x = 48;
          starting_map_y = 297;
        }
        auto cid = db.make_char(c.aid,
                                p_->name(),
                                p_->CharNum(),
                                p_->head_palette(),
                                p_->head(),
                                job,
                                p_->sex(),
                                starting_zeny,
                                starting_map,
                                starting_map_x,
                                starting_map_y);
        if (cid) {
          auto ci = db.character_info(*cid);
          if (ci) {
            c.cid = *cid;
            
            long delete_timeout{0};
            auto& i = ci->info;
            auto& s = ci->stats;
            auto& a = ci->appearance;
            auto& l = ci->location;
            if (i.delete_date) {
              auto diff = std::chrono::duration_cast<std::chrono::milliseconds>(*i.delete_date - std::chrono::system_clock::now());
              delete_timeout = diff.count();
              if (delete_timeout < 0) {
                server_state_.log()->error("Character {} of AID {} should already have been deleted for {} seconds", c.cid, c.aid, (delete_timeout / 1000) * -1);
              }
            }

            session_.emplace_and_send<packet::HC_ACCEPT_MAKECHAR>();
            session_.emplace_and_send<packet::CHARACTER_INFO>(c.cid,
                                                              s.base_exp,
                                                              s.zeny,
                                                              s.job_exp,
                                                              s.job_level,
                                                              s.body_state,  // Unused?
                                                              s.health_state, // Unused?
                                                              s.effect_state,
                                                              s.virtue,
                                                              s.honor,
                                                              s.job_point,
                                                              s.hp,
                                                              s.max_hp,
                                                              s.sp,
                                                              s.max_sp,
                                                              150, // TODO: Walk speed
                                                              i.job,
                                                              a.head,
                                                              a.body,
                                                              a.weapon,
                                                              s.base_level,
                                                              s.skill_point,
                                                              a.head_bottom, // accessory
                                                              a.shield,
                                                              a.head_top, // accessory2 
                                                              a.head_mid, // accessory3
                                                              a.head_palette,
                                                              a.body_palette,
                                                              i.name,
                                                              s.Str,
                                                              s.Agi,
                                                              s.Vit,
                                                              s.Int,
                                                              s.Dex,
                                                              s.Luk,
                                                              i.slot,
                                                              i.rename,
                                                              l.map_name,
                                                              delete_timeout,
                                                              a.robe,
                                                              i.slot,
                                                              i.rename,
                                                              i.sex
                                                              );
          } else {
            log()->error("Failed to get character info for newly created cid {} ", *cid);
            throw network::terminate_session();            
          }
        } else {
          log()->error("Failed to make character for AID {}", c.aid);
          throw network::terminate_session();            
        }
      } else {
        log()->error("Client with aid {} requested to create char with invalid job {}", c.aid, p_->job());
        throw network::terminate_session();            
      }
    } else {
      // TODO: Send "Can't use this slot"
    }
  } else {
    log()->error("Could not find account data record for aid {} in SQL database, closing client session", c.aid);
    throw network::terminate_session();            
  }

  SPDLOG_TRACE(log(), "CH_MAKE_CHAR end");
}
