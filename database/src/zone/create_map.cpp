#include "../database.hpp"

namespace ares {
  namespace database {
    namespace detail {
      struct recreate_map : pqxx::transactor<> {
        recreate_map(const std::string& name, const model::map_info& info, std::optional<uint32_t>& rslt) :
          name_(name),
          info_(info),
          rslt_(rslt) {
        }

        void operator()(argument_type& trans) {
          trans.prepared("delete_map_info_by_name")(name_).exec();
          pqxx::binarystring blob(info_.cell_flags.data(), info_.cell_flags.size());
          trans.prepared("insert_map_info")(name_)(info_.x_size)(info_.y_size)(blob).exec();
          auto qr = trans.prepared("map_id_by_name")(name_).exec();
          if (qr.size() == 1) {
            uint32_t map_id;
            qr[0]["id"].to(map_id);
            rslt_.emplace(map_id);
          }
        }

      private:
        const std::string& name_;
        const model::map_info& info_;
        std::optional<uint32_t>& rslt_;
      };
    }
  }
}

auto ares::database::db::recreate_map(const std::string& map_name, const model::map_info& map_info) -> std::optional<uint32_t> {
  SPDLOG_TRACE(log_, "database::recreate_map map_name: '{}'", map_name);
  std::optional<uint32_t> rslt;
  with_wait_lock([this, &map_name, &map_info, &rslt] () {
      detail::recreate_map t(map_name, map_info, rslt);
      pqxx_conn_->perform(t);
    });
  return rslt;
}
