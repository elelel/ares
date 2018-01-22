#include "packet_handlers.hpp"

#include "state.hpp"
#include "../server.hpp"

void ares::character::account_server::packet_handler<ares::packet::ATHENA_AH_ACCOUNT_DATA_RESULT>::operator()() {
  SPDLOG_TRACE(log(), "ATHENA_AH_ACCOUNT_DATA_RESULT: begin");
  session_ptr client;
  {
    std::lock_guard<std::mutex> lock(server_.mutex());
    client = server_.client_by_aid(p_->aid());
  }
  if (client != nullptr) {
    auto& c = client->as_client();
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
      client->emplace_and_send<packet::HC_ACCEPT_ENTER>(ad->normal_slots,
                                                   ad->premium_slots,
                                                   ad->billing_slots,
                                                   ad->creatable_slots,
                                                   ad->creatable_slots, // playable_slots = creatable slots
                                                   0);

      // Send existing characters information
      auto chars = server_.db().character_info_for_aid(p_->aid(), ad->normal_slots +
                                                       ad->premium_slots +
                                                       ad->billing_slots
                                                       );
      const uint32_t npages = chars.size() / 3 + ((chars.size() % 3) ? 1 : 0);
      client->emplace_and_send<packet::HC_CHAR_PAGES_NUM>(npages, chars.size());
      server_.log()->info("Sending {} characters for aid {}", chars.size(), p_->aid());
      client->emplace_and_send<packet::HC_CHAR_PAGES>(chars.size());
      for (const auto& ci : chars) {
        long delete_timeout{0};
        auto& i = ci.info;
        auto& s = ci.stats;
        auto& a = ci.appearance;
        auto& l = ci.location;
        if (i.delete_date) {
          auto diff = std::chrono::duration_cast<std::chrono::milliseconds>(*i.delete_date - std::chrono::system_clock::now());
          delete_timeout = diff.count();
          if (delete_timeout < 0) {
            server_.log()->error("Character {} of AID {} should already have been deleted for {} seconds", i.cid, p_->aid(), (delete_timeout / 1000) * -1);
          }
        }

        client->emplace_and_send<packet::HC_CHAR_PAGES::CHARACTER_INFO>(i.cid,
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
                                                                   0, // haircolor ?
                                                                   i.rename,
                                                                   l.map_name,
                                                                   delete_timeout,
                                                                   a.robe,
                                                                   (i.slot < ad->playable_slots) ? 1 : 0,
                                                                   (i.rename > 0) && (i.slot < ad->playable_slots) ? 1 : 0,
                                                                   i.sex
                                                                   );
      }
    } else {
      log()->error("Could not create account data record for aid {} in SQL database, closing client session", p_->aid());
      server_.remove(client);
    }
  } else {
    log()->warn("Received account data from account server for aid {}, but no session with such aid found", p_->aid());
  }
  SPDLOG_TRACE(log(), "ATHENA_AH_ACCOUNT_DATA_RESULT: end");
}
