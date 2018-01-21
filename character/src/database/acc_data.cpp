#include "database.hpp"

namespace ares {
  namespace character {
    namespace db {
      template <typename QueryResult>
      inline void assign_result(std::optional<record::acc_data>& rslt, QueryResult& qr) {
        if (qr.size() == 1) {
          record::acc_data r;
          qr[0]["normal_slots"].to(r.normal_slots);
          qr[0]["premium_slots"].to(r.premium_slots);
          qr[0]["billing_slots"].to(r.billing_slots);
          qr[0]["creatable_slots"].to(r.creatable_slots);
          qr[0]["playable_slots"].to(r.playable_slots);
          qr[0]["bank_vault"].to(r.bank_vault);
          qr[0]["max_storage"].to(r.max_storage);
          rslt.emplace(std::move(r));
        } else {
          rslt.reset();
        }
      }

      struct acc_data_for_aid : pqxx::transactor<> {
        acc_data_for_aid(const uint32_t aid, std::optional<record::acc_data>& rslt) :
          aid_(aid),
          rslt_(rslt) {
        }

        void operator()(argument_type& trans) {
          trans.conn().prepare("acc_data_for_aid", R"(
SELECT "normal_slots", "premium_slots", "billing_slots", "creatable_slots", "playable_slots", "bank_vault", "max_storage"
FROM "accounts" WHERE ("id" = $1) 
)");
          auto qr = trans.prepared("acc_data_for_aid")(aid_).exec();
          assign_result(rslt_, qr);
        }

      private:
        uint32_t aid_;
        std::optional<record::acc_data>& rslt_;
      };

      struct acc_data_create : pqxx::transactor<> {
        acc_data_create(const uint32_t aid,
                        const uint8_t normal_slots,
                        const uint8_t premium_slots,
                        const uint8_t billing_slots,
                        const uint8_t creatable_slots,
                        const uint8_t playable_slots,
                        const uint32_t bank_vault,
                        const uint32_t max_storage
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
          trans.conn().prepare("acc_data_create", R"(
INSERT INTO "accounts" ("id", "normal_slots", "premium_slots", "billing_slots", "creatable_slots", "playable_slots", "bank_vault", "max_storage")
  VALUES ($1, $2, $3, $4, $5, $6, $7, $8)
)");
          trans.prepared("acc_data_create")
            (aid_)(normal_slots_)(premium_slots_)(billing_slots_)(creatable_slots_)(playable_slots_)(bank_vault_)(max_storage_).exec();
        }
        
      private:
        const uint32_t aid_;
        const uint8_t normal_slots_;
        const uint8_t premium_slots_;
        const uint8_t billing_slots_;
        const uint8_t creatable_slots_;
        const uint8_t playable_slots_;
        const uint32_t bank_vault_;
        const uint32_t max_storage_;
      };
        
    }
  }
}

auto ares::character::database::acc_data_for_aid(const uint32_t aid) -> std::optional<db::record::acc_data> {
  std::optional<db::record::acc_data> rslt;
  with_wait_lock([this, &aid, &rslt] () {
      db::acc_data_for_aid t(aid, rslt);
      pqxx_conn_->perform(t);
    });
  return rslt;
}

void ares::character::database::acc_data_create(const uint32_t aid,
                                                const uint8_t normal_slots,
                                                const uint8_t premium_slots,
                                                const uint8_t billing_slots,
                                                const uint8_t creatable_slots,
                                                const uint8_t playable_slots,
                                                const uint32_t bank_vault,
                                                const uint32_t max_storage) {
  std::optional<db::record::acc_data> rslt;
  with_wait_lock([this, aid, normal_slots, premium_slots, billing_slots, creatable_slots, playable_slots, bank_vault, max_storage] () {
      db::acc_data_create t(aid, normal_slots, premium_slots, billing_slots, creatable_slots, playable_slots, bank_vault, max_storage);
      pqxx_conn_->perform(t);
    });
}

