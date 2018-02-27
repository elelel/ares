#include "../database.hpp"

namespace ares {
  namespace database {
    namespace detail {
      struct account_storage_for_aid : pqxx::transactor<> {
        account_storage_for_aid(const uint32_t aid, std::optional<record::account_storage>& rslt) :
          aid_(aid),
          rslt_(rslt) {
        }

        void operator()(argument_type& trans) {
          auto qr = trans.prepared("account_storage_for_aid")(aid_).exec();
          if (qr.size() == 1) {
            record::account_storage r;
            qr[0]["bank_vault"].to(r.bank_vault);
            qr[0]["max_storage"].to(r.max_storage);
            rslt_.emplace(std::move(r));
          } else {
            rslt_.reset();
          }
        }
      private:
        uint32_t aid_;
        std::optional<record::account_storage>& rslt_;
      };
    }
  }
}

auto ares::database::db::account_storage_for_aid(const uint32_t aid) -> std::optional<record::account_storage> {
  std::optional<record::account_storage> rslt;
  with_wait_lock([this, &aid, &rslt] () {
      detail::account_storage_for_aid t(aid, rslt);
      pqxx_conn_->perform(t);
    });
  return rslt;
}
