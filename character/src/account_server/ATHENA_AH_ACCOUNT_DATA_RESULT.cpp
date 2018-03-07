#include "state.hpp"
#include "../server.hpp"

namespace ares {
  struct char_pages_num {
    template <typename PacketSet, typename std::enable_if<std::is_base_of<packet_sets::pets_5a3cb64a, PacketSet>::value, int>::type = 0> 
    static void send(character::session& s, const uint32_t npages, const uint32_t nchars) {
      s.emplace_and_send<packet::current<packet::HC_CHAR_PAGES_NUM::with_nchars>>(npages, nchars);
    }

    template <typename PacketSet, typename std::enable_if<std::is_base_of<packet_sets::pets_5a4c7c5c, PacketSet>::value, int>::type = 0> 
    static void send(character::session& s, const uint32_t npages, const uint32_t) {
      s.emplace_and_send<packet::current<packet::HC_CHAR_PAGES_NUM::no_nchars>>(npages);
    }
  };
}

void ares::character::account_server::packet_handler<ares::packet::current<ares::packet::ATHENA_AH_ACCOUNT_DATA_RESULT>>::operator()() {
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

    auto ad = server_.db->query<database::accounts::slots>(p_->aid());
    if (!ad) {
      auto& conf = server_.conf();
      // Does not exist or other error, try to create default account data i
      server_.db->exec<database::accounts::create_data>(p_->aid(),
                                                        conf.normal_slots,
                                                        conf.premium_slots,
                                                        conf.billing_slots,
                                                        conf.playable_slots,
                                                        conf.creatable_slots,
                                                        conf.bank_vault,
                                                        conf.max_storage);

      // Read account data again
      ad = server_.db->query<database::accounts::slots>(p_->aid());
    }
    if (ad) {
      c.creatable_slots = ad->creatable_slots;
      c.playable_slots = ad->playable_slots;
      c.char_select_character_info = server_.db->query<database::characters::infos_for_aid>(p_->aid(), ad->normal_slots +
                                                                                            ad->premium_slots +
                                                                                            ad->billing_slots
                                                                                            );
      const uint32_t npages = c.char_select_character_info.size() > 3 ? c.char_select_character_info.size() / 3 : 1;
      
      SPDLOG_TRACE(log(), "Loaded {} characters for aid {}, Sending pages num: npages = {}, nslots = {}",
                   c.char_select_character_info.size(), p_->aid(), npages, c.creatable_slots);
      
      // Confirm account acceptance and send slots info
      SPDLOG_TRACE(log(), "sending HC_ACCEPT_ENTER");
      s->emplace_and_send<packet::current<packet::HC_ACCEPT_ENTER>>(ad->normal_slots,
                                                                    ad->premium_slots,
                                                                    ad->billing_slots,
                                                                    ad->creatable_slots,
                                                                    ad->playable_slots,
                                                                    0);

      SPDLOG_TRACE(log(), "sending HC_ACCEPT_ENTER done");
      // Send existing characters information
      char_pages_num::send<packet_sets::current>(*s, npages, c.creatable_slots);
      s->emplace_and_send<packet::current<packet::HC_BLOCK_CHARACTER>>();
    } else {
      log()->error("Could not create account data record for aid {} in SQL database, closing session", p_->aid());
      s->close_gracefuly();
    }
  } else {
    log()->warn("Received account data from account server for aid {}, but no session with such aid found", p_->aid());
  }
  SPDLOG_TRACE(log(), "ATHENA_AH_ACCOUNT_DATA_RESULT: end");
}

