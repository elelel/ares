#include "database.hpp"

namespace ares {
  namespace zone {
    namespace db {
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

auto ares::zone::database::whole_map_index() -> std::vector<db::record::map_index> {
  std::vector<db::record::map_index> rslt;
  with_wait_lock([this, &rslt] () {
      db::whole_map_index t(rslt);
      pqxx_conn_->perform(t);
    });
  return rslt;
}
