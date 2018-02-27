#include "../database.hpp"

namespace ares {
  namespace database {
    namespace detail {
      struct account_slots_for_aid : pqxx::transactor<> {
        account_slots_for_aid(const uint32_t aid, std::optional<record::account_slots>& rslt) :
          aid_(aid),
          rslt_(rslt) {
        }
        
        void operator()(argument_type& trans) {
          auto qr = trans.prepared("account_slots_for_aid")(aid_).exec();
          if (qr.size() == 1) {
            record::account_slots r;
            qr[0]["normal_slots"].to(r.normal_slots);
            qr[0]["premium_slots"].to(r.premium_slots);
            qr[0]["billing_slots"].to(r.billing_slots);
            qr[0]["creatable_slots"].to(r.creatable_slots);
            qr[0]["playable_slots"].to(r.playable_slots);
            rslt_.emplace(std::move(r));
          } else {
            rslt_.reset();
          }
        }

      private:
        uint32_t aid_;
        std::optional<record::account_slots>& rslt_;
      };

    }
  }
}

auto ares::database::db::account_slots_for_aid(const uint32_t aid) -> std::optional<record::account_slots> {
  std::optional<record::account_slots> rslt;
  with_wait_lock([this, &aid, &rslt] () {
      detail::account_slots_for_aid t(aid, rslt);
      pqxx_conn_->perform(t);
    });
  return rslt;
}

