#include <ares/common>

#include "state.hpp"
#include "../server.hpp"

void ares::character::client::packet_handler<ares::packet::current<ares::packet::CH_MAKE_CHAR::no_stats>>::operator()() {
  SPDLOG_TRACE(log(), "CH_MAKE_CHAR begin");
  auto& c = session_.as_client();
  auto& db = server_.db;
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

        uint32_t starting_map_id{0};
        {
          std::lock_guard<std::mutex> lock(server_.mutex());
          starting_map_id = server_.map_id_by_map_name(starting_map);
        }
        if (starting_map_id != 0) {
          auto cid = db.make_char(c.aid,
                                  std::string(p_->name()),
                                  p_->CharNum(),
                                  p_->head_palette(),
                                  p_->head(),
                                  job,
                                  p_->sex(),
                                  starting_zeny,
                                  starting_map_id,
                                  starting_map_x,
                                  starting_map_y);
          if (cid) {
            state_.char_info = db.character_info(*cid);
            auto &ci = state_.char_info;
            if (ci) {
              long delete_timeout{0};
              if (ci->delete_date) {
                auto diff = std::chrono::duration_cast<std::chrono::milliseconds>(*ci->delete_date - std::chrono::system_clock::now());
                delete_timeout = diff.count();
                if (delete_timeout < 0) {
                  log()->error("Character {} of AID {} should already have been deleted for {} seconds", ci->pc.cid, ci->pc.aid, (delete_timeout / 1000) * -1);
                }
              }

              std::lock_guard<std::mutex> lock(server_.mutex());
              const auto& last_map_name = server_.map_name_by_map_id(ci->pc.location_last.map_id);
              if (last_map_name.size() > 0) {
                session_.emplace_and_send<packet::current<packet::HC_ACCEPT_MAKECHAR>>();
                session_.emplace_and_send<packet::CHARACTER_INFO>(ci->pc,
                                                                  last_map_name,
                                                                  delete_timeout,
                                                                  1,  // TODO: Change slot enabled
                                                                  1  // TODO: Rename enabled
                                                                  );
              } else {
                log()->error("Make char failed: last map id {} was not found in id to map name index", ci->pc.location_last.map_id);
                session_.close_gracefuly();
              }
            } else {
              log()->error("Failed to get character info for newly created cid {} ", *cid);
              session_.close_gracefuly();
            }
          } else {
            log()->error("Failed to make character for AID {}", c.aid);
            session_.close_gracefuly();
          }
        } else {
          log()->error("Failed to make character, map '{}' is not configured", starting_map);
          session_.close_gracefuly();
        }
      } else {
        log()->error("Client with aid {} requested to create char with invalid job {}", c.aid, p_->job());
        session_.close_gracefuly();
      }
    } else {
      // TODO: Send "Can't use this slot"
    }
  } else {
    log()->error("Could not find account data record for aid {} in SQL database, closing client session", c.aid);
    session_.close_gracefuly();
  }

  SPDLOG_TRACE(log(), "CH_MAKE_CHAR end");
}
