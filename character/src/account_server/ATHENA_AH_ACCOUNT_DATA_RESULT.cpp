#include "packet_handlers.hpp"

#include "state.hpp"
#include "../server.hpp"

void ares::character::account_server::packet_handler<ares::packet::ATHENA_AH_ACCOUNT_DATA_RESULT>::operator()() {
  SPDLOG_TRACE(log(), "ATHENA_AH_ACCOUNT_DATA_RESULT: begin");
  session_ptr s;
  {
    std::lock_guard<std::mutex> lock(server_.mutex());
    s = server_.client_by_aid(p_->aid());
  }
  if (s != nullptr) {
    auto& c = s->as_client();
    c.email = p_->email();
    c.expiration_time = p_->expiration_time();
    c.gmlevel = p_->gmlevel();
    c.pin = p_->pin();
    // TODO: birthdate conversion

    auto ad = server_.db().account_slots_for_aid(p_->aid());
    if (!ad) {
      // Does not exist or other error, try to create default account data i
      server_.db().account_create(p_->aid(),
                                  server_.conf().normal_slots,
                                  server_.conf().premium_slots,
                                  server_.conf().billing_slots,
                                  server_.conf().playable_slots,
                                  server_.conf().creatable_slots,
                                  server_.conf().bank_vault,
                                  server_.conf().max_storage);

      // Read account data again
      ad = server_.db().account_slots_for_aid(p_->aid());
    }
    if (ad) {
      // Confirm account acceptance and send slots info
      s->emplace_and_send<packet::HC_ACCEPT_ENTER>(ad->normal_slots,
                                                   ad->premium_slots,
                                                   ad->billing_slots,
                                                   ad->creatable_slots,
                                                   ad->creatable_slots, // playable_slots = creatable slots
                                                   0);

      // Send existing characters information
      auto chars = server_.db().char_data_for_aid(p_->aid(), ad->normal_slots +
                                                  ad->premium_slots +
                                                  ad->billing_slots
                                                  );
      const uint32_t npages = chars.size() / 3 + ((chars.size() % 3) ? 1 : 0);
      s->emplace_and_send<packet::HC_CHAR_PAGES_NUM>(npages, chars.size());
      server_.log()->info("Sending {} characters for aid {}", chars.size(), p_->aid());
      s->emplace_and_send<packet::HC_CHAR_PAGES>(chars.size());
      for (const auto& c : chars) {
        long delete_timeout{0};
        if (c.delete_date) {
          auto diff = std::chrono::duration_cast<std::chrono::milliseconds>(*c.delete_date - std::chrono::system_clock::now());
          delete_timeout = diff.count();
          if (delete_timeout < 0) {
            server_.log()->error("Character {} of AID {} should already have been deleted for {} seconds", c.cid, p_->aid(), (delete_timeout / 1000) * -1);
          }
        }

        auto sex = c.sex;
        if (sex == 99) {
          // TODO: Sex should be obtained from account information, obsolete?
        }
        s->emplace_and_send<packet::HC_CHAR_PAGES::CHARACTER_INFO>(c.cid,
                                                                   c.base_exp,
                                                                   c.zeny,
                                                                   c.job_exp,
                                                                   c.job_level,
                                                                   c.body_state,  // Unused?
                                                                   c.health_state, // Unused?
                                                                   c.effect_state,
                                                                   c.virtue,
                                                                   c.honor,
                                                                   c.job_point,
                                                                   c.hp,
                                                                   c.max_hp,
                                                                   c.sp,
                                                                   c.max_sp,
                                                                   150, // TODO: Walk speed
                                                                   c.job,
                                                                   c.head,
                                                                   c.body,
                                                                   c.weapon,
                                                                   c.base_level,
                                                                   c.skill_point,
                                                                   c.head_bottom, // accessory
                                                                   c.shield,
                                                                   c.head_top, // accessory2 
                                                                   c.head_mid, // accessory3
                                                                   c.head_palette,
                                                                   c.body_palette,
                                                                   c.name,
                                                                   c.Str,
                                                                   c.Agi,
                                                                   c.Vit,
                                                                   c.Int,
                                                                   c.Dex,
                                                                   c.Luk,
                                                                   c.slot,
                                                                   0, // haircolor ?
                                                                   c.rename,
                                                                   c.last_map_name,
                                                                   delete_timeout,
                                                                   c.robe,
                                                                   (c.slot < ad->playable_slots) ? 1 : 0,
                                                                   (c.rename > 0) && (c.slot < ad->playable_slots) ? 1 : 0,
                                                                   sex
                                                                   );
      }
    } else {
      log()->error("Could not create account data record for aid {} in SQL database, closing client session", p_->aid());
      server_.remove(s);
    }
  } else {
    log()->warn("Received account data from account server for aid {}, but no session with such aid found", p_->aid());
  }
  SPDLOG_TRACE(log(), "ATHENA_AH_ACCOUNT_DATA_RESULT: end");
}
