#include "../database.hpp"

namespace ares {
  namespace database {
    namespace detail {
      struct map_name_index : pqxx::transactor<> {
        map_name_index(std::vector<record::map_index>& rslt) :
          rslt_(rslt) {
        }

        void operator()(argument_type& trans) {
          rslt_.clear();
          auto qr = trans.prepared("map_name_index").exec();
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

      struct map_id_by_name : pqxx::transactor<> {
        map_id_by_name(const std::string& name, std::optional<uint32_t>& rslt) :
          name_(name),
          rslt_(rslt) {
        }

        void operator()(argument_type& trans) {
          auto qr = trans.prepared("map_id_by_name")(name_).exec();
          if (qr.size() == 1) {
            uint32_t id;
            qr[0]["id"].to(id);
            rslt_.emplace(id);
          }
        }
      private:
        const std::string& name_;
        std::optional<uint32_t>& rslt_;
      };
    }
  }
}

auto ares::database::db::map_name_index() -> std::vector<record::map_index> {
  std::vector<record::map_index> rslt;
  with_wait_lock([this, &rslt] () {
      detail::map_name_index t(rslt);
      pqxx_conn_->perform(t);
    });
  return rslt;
}

auto ares::database::db::map_id_by_name(const std::string& name) -> std::optional<uint32_t> {
  std::optional<uint32_t> rslt;
  with_wait_lock([this, &name, &rslt] () {
      detail::map_id_by_name t(name, rslt);
      pqxx_conn_->perform(t);
    });
  return rslt;
}
