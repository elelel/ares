#include "../database.hpp"

namespace ares {
  namespace database {
    namespace detail {
      struct whole_map_index : pqxx::transactor<> {
        whole_map_index(std::vector<record::map_index>& rslt) :
          rslt_(rslt) {
        }

        void operator()(argument_type& trans) {
          rslt_.clear();
          auto qr = trans.prepared("whole_map_index").exec();
          for (const auto& row : qr) {
            record::map_index r;
            row["id"].to(r.id);
            row["external_id"].to(r.external_id);
            row["name"].to(r.name);
            rslt_.push_back(std::move(r));
          }
        }

      private:
        std::vector<record::map_index>& rslt_;
      };
    }
  }
}

auto ares::database::db::whole_map_index() -> std::vector<record::map_index> {
  std::vector<record::map_index> rslt;
  with_wait_lock([this, &rslt] () {
      detail::whole_map_index t(rslt);
      pqxx_conn_->perform(t);
    });
  return rslt;
}
