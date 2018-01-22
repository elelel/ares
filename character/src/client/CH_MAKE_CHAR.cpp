#include "packet_handlers.hpp"

#include <ares/common>

#include "state.hpp"
#include "../server.hpp"

void ares::character::client::packet_handler<ares::packet::CH_MAKE_CHAR>::operator()() {
  SPDLOG_TRACE(log(), "CH_MAKE_CHAR begin");
  auto& c = session_.as_client();
  auto slots = server_.db().account_slots_for_aid(c.aid);
  if (slots) {
    // TODO: Check if new char creation is allowed in server configuration
    if (p_->CharNum() < slots->creatable_slots) {
      // TODO: Check if slot is not used
      const auto job = static_cast<ares::JOB>(p_->job());
      if ((job == JOB::NOVICE) || (job == JOB::SUMMONER)) {
        // TODO: Starting zeny, locations from server config
        const uint32_t starting_zeny = 1000;
        std::string starting_map{"prontera"};
        uint16_t starting_map_x{150};
        uint16_t starting_map_y{150};
        if (job == JOB::SUMMONER) {
          starting_map = "lasa_fild01";
          starting_map_x = 48;
          starting_map_y = 297;
        }
        auto cid = server_.db().make_char(c.aid,
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
          c.cid = *cid;
          // TODO: Load data from sql
          // TODO: Send data in HC_ACCEPT_MAKECHAR
        } else {
          log()->error("Failed to make character for AID {}", c.aid);
          server_.remove(session_.shared_from_this());
        }
      } else {
        log()->error("Client with aid {} requested to create char with invalid job {}", c.aid, p_->job());
        server_.remove(session_.shared_from_this());
      }
    } else {
      // TODO: Send "Can't use this slot"
    }
  } else {
    log()->error("Could not find account data record for aid {} in SQL database, closing client session", c.aid);
    server_.remove(session_.shared_from_this());
  }

  SPDLOG_TRACE(log(), "CH_MAKE_CHAR end");
}
