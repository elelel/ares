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

    auto ad = server_.db().acc_data_for_aid(p_->aid());
    if (!ad) {
      // Does not exist or other error, try to create default account data i
      server_.db().acc_data_create(p_->aid(),
                                   server_.conf().normal_slots,
                                   server_.conf().premium_slots,
                                   server_.conf().billing_slots,
                                   server_.conf().playable_slots,
                                   server_.conf().creatable_slots,
                                   server_.conf().bank_vault,
                                   server_.conf().max_storage);

      // Read account data again
                                   ad = server_.db().acc_data_for_aid(p_->aid());
      if (!ad) {
        log()->error("Could not create account data record for aid {} in SQL database", p_->aid());
        server_.remove(s);
      }
    }
    session_.emplace_and_send<packet::HC_ACCEPT_ENTER>(ad->normal_slots,
                                                       ad->premium_slots,
                                                       ad->billing_slots,
                                                       ad->creatable_slots,
                                                       ad->creatable_slots, // playable_slots = creatable slots
                                                       0);
    
      auto chars = server_.db().char_data_for_aid(p_->aid(), ad->normal_slots +
                                                  ad->premium_slots +
                                                  ad->billing_slots
                                                  );
  } else {
    log()->warn("Received account data from account server for aid {}, but no session with such aid found", p_->aid());
  }
  SPDLOG_TRACE(log(), "ATHENA_AH_ACCOUNT_DATA_RESULT: end");
}
