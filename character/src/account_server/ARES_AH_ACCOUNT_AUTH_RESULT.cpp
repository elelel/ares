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

void ares::character::account_server::packet_handler<ares::packet::current<ares::packet::ARES_AH_ACCOUNT_AUTH_RESULT>>::operator()() {
  SPDLOG_TRACE(log(), "ARES_AH_ACCOUNT_AUTH_RESULT: begin");
  std::shared_ptr<session> s;
  {
    std::lock_guard<std::mutex> lock(server_.mutex());
    s = server_.auth_requests->responded(p_->request_id());
  }
  if (s) {
    if (p_->result() == 0) {
      if (s->is_mono()) {
        server_.log()->info("Account {} was authenticated by account server", p_->account_id().to_string());

        
        auto new_state = s->as_mono();
        s->session_state_.emplace<client::state>(std::move(new_state));
        auto& c = s->as_client();
        c.account_id = p_->account_id();
        server_.add(s);


        auto ad = server_.db->query<database::accounts::slots>(p_->account_id());
        if (!ad) {
          auto& conf = server_.conf();
          // Does not exist or other error, try to create default account data i
          server_.db->exec<database::accounts::create_data>(p_->account_id(),
                                                            conf.normal_slots,
                                                            conf.premium_slots,
                                                            conf.billing_slots,
                                                            conf.playable_slots,
                                                            conf.creatable_slots,
                                                            conf.bank_vault,
                                                            conf.max_storage);

          // Read account data again
          ad = server_.db->query<database::accounts::slots>(p_->account_id());
        }
        if (ad) {
          c.creatable_slots = ad->creatable_slots;
          c.playable_slots = ad->playable_slots;
          c.char_select_character_info = server_.db->query<database::characters::infos_for_account_id>(p_->account_id(), ad->normal_slots +
                                                                                                       ad->premium_slots +
                                                                                                       ad->billing_slots
                                                                                                       );
          const uint32_t npages = c.char_select_character_info.size() > 3 ? c.char_select_character_info.size() / 3 : 1;
      
          SPDLOG_TRACE(log(), "Loaded {} characters for account {}, Sending pages num: npages = {}, nslots = {}",
                       c.char_select_character_info.size(), p_->account_id().to_string(), npages, c.creatable_slots);
      
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
          log()->error("Could not create account data record for account {} in SQL database, closing session", p_->account_id().to_string());
          s->close_gracefuly();
        }
        
      } else {
        log()->error("Received authentication result from account server for account {}, but it's session is not in mono state, ignoring.", p_->account_id().to_string());
      }
    } else {
      log()->warn("Account {} authentication denied by account server, closing session!", p_->account_id().to_string());
      s->emplace_and_send<packet::current<packet::AC_REFUSE_LOGIN>>(8); // Rejected by server
      s->close_gracefuly();
    }
  } else {
    log()->error("Received authentication result from account server for account {}, but auth request manager was not expecting it, ignoring.", p_->account_id().to_string());
  }
}
