#include "../database.hpp"

namespace ares {
  namespace database {
    namespace detail {
      struct map_info : pqxx::transactor<> {
        map_info(const uint32_t id, std::optional<model::map_info>& rslt) :
          id_(id),
          rslt_(rslt) {
        }

        void operator()(argument_type& trans) {
          auto qr = trans.prepared("map_info")(id_).exec();
          if (qr.size() == 1) {
            model::map_info r;
            qr[0]["x_size"].to(r.x_size);
            qr[0]["y_size"].to(r.y_size);
            pqxx::binarystring blob(qr[0]["cell_flags"]);
            for (const auto& f : blob) {
              r.cell_flags.push_back(model::map_cell_flags::from_uint8(f));
            }
            rslt_.emplace(std::move(r));
          }
        }
      private:
        uint32_t id_;
        std::optional<model::map_info>& rslt_;
      };

      struct save_map_info : pqxx::transactor<> {
        save_map_info(const uint32_t id, const model::map_info& info) :
          id_(id),
          info_(info) {
        }

        void operator()(argument_type& trans) {
          trans.prepared("delete_map_info")(id_).exec();
          pqxx::binarystring blob(info_.cell_flags.data(), info_.cell_flags.size());
          trans.prepared("insert_map_info")(id_)(info_.x_size)(info_.y_size)(blob).exec();
        }

      private:
        uint32_t id_;
        const model::map_info& info_;
      };
    }
  }
}


auto ares::database::db::map_info(const size_t map_id) -> std::optional<model::map_info> {
  std::optional<model::map_info> rslt;
  with_wait_lock([this, map_id, &rslt] () {
      detail::map_info t(map_id, rslt);
      pqxx_conn_->perform(t);
    });
  return rslt;
}

void ares::database::db::save_map_info(const size_t map_id, const model::map_info& map_info) {
  with_wait_lock([this, &map_id, &map_info] () {
      detail::save_map_info t(map_id, map_info);
      pqxx_conn_->perform(t);
    });
}
