#include "../database.hpp"

namespace ares {
  namespace database {
    namespace detail {
      struct account_create : pqxx::transactor<> {
        account_create(const uint32_t& aid,
                       const uint8_t& normal_slots,
                       const uint8_t& premium_slots,
                       const uint8_t& billing_slots,
                       const uint8_t& creatable_slots,
                       const uint8_t& playable_slots,
                       const uint32_t& bank_vault,
                       const uint32_t& max_storage
                       ) :
          aid_(aid),
          normal_slots_(normal_slots),
          premium_slots_(premium_slots),
          billing_slots_(billing_slots),
          creatable_slots_(creatable_slots),
          playable_slots_(playable_slots),
          bank_vault_(bank_vault),
          max_storage_(max_storage) {
        }

        void operator()(argument_type& trans) {
          trans.prepared("account_create")
            (aid_)(normal_slots_)(premium_slots_)(billing_slots_)(creatable_slots_)(playable_slots_)
            (bank_vault_)(max_storage_).exec();
        }

      private:
        const uint32_t& aid_;
        const uint8_t& normal_slots_;
        const uint8_t& premium_slots_;
        const uint8_t& billing_slots_;
        const uint8_t& creatable_slots_;
        const uint8_t& playable_slots_;
        const uint32_t& bank_vault_;
        const uint32_t& max_storage_;
      };
    };
  }
}

void ares::database::db::account_create(const uint32_t aid,
                                        const uint8_t normal_slots,
                                        const uint8_t premium_slots,
                                        const uint8_t billing_slots,
                                        const uint8_t creatable_slots,
                                        const uint8_t playable_slots,
                                        const uint32_t bank_vault,
                                        const uint32_t max_storage) {
  with_wait_lock([this, &aid, &normal_slots, &premium_slots, &billing_slots, &creatable_slots, &playable_slots, &bank_vault, &max_storage] () {
      detail::account_create t(aid, normal_slots, premium_slots, billing_slots, creatable_slots, playable_slots, bank_vault, max_storage);
      pqxx_conn_->perform(t);
    });
}
