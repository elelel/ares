#include "../accounts.hpp"

ares::database::accounts::create_data::create_data(const model::account_id& account_id,
                                                   const uint8_t& normal_slots,
                                                   const uint8_t& premium_slots,
                                                   const uint8_t& billing_slots,
                                                   const uint8_t& creatable_slots,
                                                   const uint8_t& playable_slots,
                                                   const uint32_t& bank_vault,
                                                   const uint32_t& max_storage
                                                   ) :
  account_id_(account_id),
  normal_slots_(normal_slots),
  premium_slots_(premium_slots),
  billing_slots_(billing_slots),
  creatable_slots_(creatable_slots),
  playable_slots_(playable_slots),
  bank_vault_(bank_vault),
  max_storage_(max_storage) {
  }

void ares::database::accounts::create_data::operator()(argument_type& trans) {
  trans.prepared("account_create_data")
    (account_id_)(normal_slots_)(premium_slots_)(billing_slots_)(creatable_slots_)(playable_slots_)
    (bank_vault_)(max_storage_).exec();
}
