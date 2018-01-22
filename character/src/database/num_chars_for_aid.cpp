#include "database.hpp"

namespace ares {
  namespace character {
    namespace db {
      struct num_chars_for_aid : pqxx::transactor<> {
        num_chars_for_aid(const uint32_t& aid, const size_t& max_chars, std::optional<size_t>& rslt) :
          aid_(aid),
          max_chars_(max_chars),
          rslt_(rslt) {
        }

        void operator()(argument_type& trans) {
          rslt_.reset();
          auto qr = trans.prepared("num_chars_for_aid")(aid_)(max_chars_).exec();
          if (qr.size() == 1) {
            size_t cnt;
            qr[0]["cnt"].to(cnt);
            rslt_.emplace(cnt);
          }
        }
        
      private:
        const uint32_t& aid_;
        const size_t& max_chars_;
        std::optional<size_t>& rslt_;
      };
    }
  }
}

auto ares::character::database::num_chars_for_aid(const uint32_t aid, const size_t max_chars) -> std::optional<size_t> {
  std::optional<size_t> rslt;
  with_wait_lock([this, &aid, &max_chars, &rslt] () {
      db::num_chars_for_aid t(aid, max_chars, rslt);
      pqxx_conn_->perform(t);
    });
  return rslt;
}
