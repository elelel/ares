#include "../database.hpp"

namespace ares {
  namespace database {
    namespace detail {
      struct char_delete_date : pqxx::transactor<> {
        char_delete_date(const uint32_t& cid, std::optional<std::chrono::system_clock::time_point>& rslt) :
          cid_(cid),
          rslt_(rslt) {
        }

        void operator()(argument_type& trans) {
          rslt_.reset();
          auto qr = trans.prepared("char_delete_date")(cid_).exec();
          if (qr.size() == 1) {
            qr[0]["delete_date"].to(rslt_);
          }
        }

      private:
        uint32_t cid_;
        std::optional<std::chrono::system_clock::time_point>& rslt_;
      };
    }
  }
}

auto ares::database::db::char_delete_date(const uint32_t cid) -> std::optional<std::chrono::system_clock::time_point> {
  std::optional<std::chrono::system_clock::time_point> rslt;
  with_wait_lock([this, cid, &rslt] () {
      detail::char_delete_date t(cid, rslt);
      pqxx_conn_->perform(t);
    });
  return rslt;
}

