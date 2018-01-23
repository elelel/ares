#include "packet_handlers.hpp"

#include <ares/common>

#include "state.hpp"
#include "../server.hpp"

void ares::character::client::packet_handler<ares::packet::CH_CHAR_PAGE_REQ>::operator()() {
  SPDLOG_TRACE(log(), "CH_CHAR_PAGE_REQ begin");
  auto& c = session_.as_client();
  auto& unsent_chars = c.char_select_character_info;
  if (unsent_chars.size() > 0) {
    size_t num_to_send = 3;
    if (unsent_chars.size() < 3) num_to_send = unsent_chars.size();
    for (size_t k = 0; k < num_to_send; ++k) {
      session_.emplace_and_send<packet::HC_CHAR_PAGES>(num_to_send);
      const auto& ci = unsent_chars[k];
      long delete_timeout{0};
      auto& i = ci.info;
      auto& s = ci.stats;
      auto& a = ci.appearance;
      auto& l = ci.location;
      if (i.delete_date) {
        auto diff = std::chrono::duration_cast<std::chrono::milliseconds>(*i.delete_date - std::chrono::system_clock::now());
        delete_timeout = diff.count();
        if (delete_timeout < 0) {
          server_.log()->error("Character {} of AID {} should already have been deleted for {} seconds", i.cid, c.aid, (delete_timeout / 1000) * -1);
        }
      }

      server_.log()->info("Sending charinfo {}", sizeof(packet::CHARACTER_INFO));

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
  }
  SPDLOG_TRACE(log(), "CH_CHAR_PAGE_REQ end");
}
