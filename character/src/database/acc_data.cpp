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
          trans.conn().prepare("acc_data", R"(
SELECT "normal_slots", "premium_slots", "billing_slots", "creatable_slots", "playable_slots", "bank_vault", "max_storage",
FROM "accounts" WHERE ("id" = $1) 
)");
          auto qr = trans.prepared("acc_data")(aid_).exec();
          assign_result(rslt_, qr);
        }

      private:
        uint32_t aid_;
        std::optional<record::acc_data>& rslt_;
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
