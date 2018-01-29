#include <ares/common>

#include "state.hpp"
#include "../state.hpp"

void ares::character::client::packet_handler<ares::packet::CH_CHAR_PAGE_REQ>::operator()() {
  SPDLOG_TRACE(log(), "CH_CHAR_PAGE_REQ begin");
  auto& c = session_.as_client();
  auto& unsent_chars = c.char_select_character_info;
  SPDLOG_TRACE(log(), "Got char page req, unsent chars {}", unsent_chars.size());

  if (unsent_chars.size() > 0) {
    size_t num_to_send = 3;
    if (unsent_chars.size() < 3) num_to_send = unsent_chars.size();
    session_.emplace_and_send<packet::HC_CHAR_PAGES>(num_to_send);
    for (size_t k = 0; k < num_to_send; ++k) {
      const auto& ci = unsent_chars[unsent_chars.size() - 1];
      long delete_timeout{0};
      auto& i = ci.info;
      auto& s = ci.stats;
      auto& a = ci.appearance;
      auto& l = ci.location;
      if (i.delete_date) {
        auto diff = std::chrono::duration_cast<std::chrono::milliseconds>(*i.delete_date - std::chrono::system_clock::now());
        delete_timeout = diff.count();
        if (delete_timeout < 0) {
          server_state_.log()->error("Character {} of AID {} should already have been deleted for {} seconds", i.cid, c.aid, (delete_timeout / 1000) * -1);
        }
      }

      SPDLOG_TRACE(server_state_.log(), "Sending character {} in response to char page req", i.cid);

      session_.emplace_and_send<packet::CHARACTER_INFO>(i.cid,
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
                                                        (i.slot < c.playable_slots) ? 1 : 0,
                                                        (i.rename > 0) && (i.slot < c.playable_slots) ? 1 : 0,
                                                        i.sex
                                                        );
      
      unsent_chars.pop_back();
    }
    if (unsent_chars.size() % 3 == 0) {
      // Send page terminator if we filled the whole page
      session_.emplace_and_send<packet::HC_CHAR_PAGES>(0);
    }
    if (unsent_chars.size() == 0) {
      c.char_select_all_sent = true;
    }
  } else {
    if (!c.char_select_all_sent) {
      // Send page terminator if there were no chars at all
      session_.emplace_and_send<packet::HC_CHAR_PAGES>(0);
      c.char_select_all_sent = true;
    }
  }
  SPDLOG_TRACE(log(), "CH_CHAR_PAGE_REQ end");
}
