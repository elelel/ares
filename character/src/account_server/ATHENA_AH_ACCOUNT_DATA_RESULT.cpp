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
      c.playable_slots = ad->playable_slots;
      // Confirm account acceptance and send slots info
      client->emplace_and_send<packet::HC_ACCEPT_ENTER>(ad->normal_slots,
                                                        ad->premium_slots,
                                                        ad->billing_slots,
                                                        ad->creatable_slots,
                                                        ad->playable_slots,
                                                        0);

      // Send existing characters information
      c.char_select_character_info = server_.db().character_info_for_aid(p_->aid(), ad->normal_slots +
                                                                         ad->premium_slots +
                                                                         ad->billing_slots
                                                                         );
      
      //      const auto nchars = c.char_select_character_info.size();
      
      const uint32_t npages = (ad->creatable_slots / 3) + ((ad->creatable_slots % 3) != 0 ? 1 : 0);
      log()->info("Sending pages num: npages = {}, nslots = {}", npages, ad->creatable_slots);
      client->emplace_and_send<packet::HC_CHAR_PAGES_NUM>(npages, ad->creatable_slots);
      client->emplace_and_send<packet::HC_BLOCK_CHARACTER>();
    } else {
      log()->error("Could not create account data record for aid {} in SQL database, closing client session", p_->aid());
      server_.remove(client);
    }
  } else {
    log()->warn("Received account data from account server for aid {}, but no session with such aid found", p_->aid());
  }
  SPDLOG_TRACE(log(), "ATHENA_AH_ACCOUNT_DATA_RESULT: end");
}
