#include "state.hpp"
#include "../state.hpp"

void ares::character::account_server::packet_handler<ares::packet::ATHENA_AH_ACCOUNT_DATA_RESULT>::operator()() {
  SPDLOG_TRACE(log(), "ATHENA_AH_ACCOUNT_DATA_RESULT: begin");
  auto& server = server_state_.server;
  session_ptr s;
  {
    std::lock_guard<std::mutex> lock(server.mutex());
    s = server.client_by_aid(p_->aid());
  }
  if (s != nullptr) {
    auto& c = s->as_client();
    c.email = p_->email();
    c.expiration_time = p_->expiration_time();
    c.gmlevel = p_->gmlevel();
    c.pin = p_->pin();
    // TODO: birthdate conversion

    auto ad = server_state_.db.account_slots_for_aid(p_->aid());
    if (!ad) {
      auto& conf = server_state_.conf;
      // Does not exist or other error, try to create default account data i
      server_state_.db.account_create(p_->aid(),
                                 conf.normal_slots,
                                  conf.premium_slots,
                                  conf.billing_slots,
                                  conf.playable_slots,
                                  conf.creatable_slots,
                                  conf.bank_vault,
                                  conf.max_storage);

      // Read account data again
      ad = server_state_.db.account_slots_for_aid(p_->aid());
    }
    if (ad) {
      c.creatable_slots = ad->creatable_slots;
      c.playable_slots = ad->playable_slots;
      // Confirm account acceptance and send slots info
      SPDLOG_TRACE(log(), "sending HC_ACCEPT_ENTER");
      s->emplace_and_send<packet::HC_ACCEPT_ENTER>(ad->normal_slots,
                                                   ad->premium_slots,
                                                   ad->billing_slots,
                                                   ad->creatable_slots,
                                                   ad->playable_slots,
                                                   0);

      SPDLOG_TRACE(log(), "sending HC_ACCEPT_ENTER done");
      // Send existing characters information
      c.char_select_character_info = server_state_.db.character_info_for_aid(p_->aid(), ad->normal_slots +
                                                                             ad->premium_slots +
                                                                             ad->billing_slots
                                                                             );
      const uint32_t nchars = 50;
      const uint32_t npages = (nchars / 3) + ((nchars % 3) != 0 ? 1 : 0);
      
      SPDLOG_TRACE(log(), "Loaded {} characters for aid {}, Sending pages num: npages = {}, nslots = {}",
                   c.char_select_character_info.size(), p_->aid(), npages, c.creatable_slots);
      s->emplace_and_send<packet::HC_CHAR_PAGES_NUM>(npages, c.creatable_slots);
      s->emplace_and_send<packet::HC_BLOCK_CHARACTER>();
    } else {
      log()->error("Could not create account data record for aid {} in SQL database, closing s session", p_->aid());
      std::lock_guard<std::mutex> lock(server.mutex());
      s->remove_from_server();
    }
  } else {
    log()->warn("Received account data from account server for aid {}, but no session with such aid found", p_->aid());
  }
  SPDLOG_TRACE(log(), "ATHENA_AH_ACCOUNT_DATA_RESULT: end");
}
