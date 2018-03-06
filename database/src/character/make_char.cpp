#include "../database.hpp"

namespace ares {
  namespace database {
    namespace detail {
      struct make_char : pqxx::transactor<> {
        make_char(const uint32_t& aid,
                  const std::string& name,
                  const uint8_t& slot,
                  const uint16_t& head_palette,
                  const uint16_t& head,
                  const model::pc_job& job,
                  const uint8_t& sex,
                  const uint64_t& zeny,
                  const uint32_t& map_id,
                  const uint16_t& map_x,
                  const uint16_t& map_y,
                  std::optional<uint32_t>& rslt) :
          aid_(aid),
          name_(name),
          slot_(slot),
          head_palette_(head_palette),
          head_(head),
          job_(job),
          sex_(sex),
          zeny_(zeny),
          map_id_(map_id),
          map_x_(map_x),
          map_y_(map_y),
          rslt_(rslt) {
        }

        void operator()(argument_type& trans) {
          rslt_.reset();
          // Int, Vit, etc. are first fixed to 1 after modern CH_MAKE_CHAR packet
          const auto Vit = 1;
          const auto Int = 1;
          uint32_t hp = 40 * (100 + Vit) / 100;
          uint32_t sp = 11 * (100 + Int) / 100;
          if (job_ == model::pc_job::SUMMONER) {
            hp = 60 * (100 + Vit) / 100;
            sp = 8 * (100 + Int) / 100;
          }
          
          trans.prepared("make_char_create_cid")
            (aid_)(slot_)(name_)(sex_)(static_cast<uint16_t>(job_)).exec();

          auto create_cid = trans.prepared("cid_by_name")(name_).exec();
          
          if (create_cid.size() == 1) {
            uint32_t cid;
            create_cid[0]["id"].to(cid);
            
            trans.prepared("make_char_create_stats")
              (cid)(hp)(sp).exec();

            trans.prepared("make_char_create_zeny")
              (cid)(zeny_).exec();
            
            trans.prepared("make_char_create_appearance")
              (cid)(head_)(head_palette_).exec();

            trans.prepared("make_char_create_last_location")
              (cid)(map_id_)(map_x_)(map_y_).exec();

            trans.prepared("make_char_create_save_location")
              (cid)(map_id_)(map_x_)(map_y_).exec();
            
            // TODO: Add items (weapon/armour)

            rslt_.emplace(cid);
          } else {
            trans.abort();
          }
        }

      private:
        const uint32_t& aid_;
        const std::string& name_;
        const uint8_t& slot_;
        const uint16_t& head_palette_;
        const uint16_t& head_;
        const model::pc_job& job_;
        const uint8_t& sex_;
        const uint64_t& zeny_;
        const uint32_t& map_id_;
        const uint16_t& map_x_;
        const uint16_t& map_y_;

        std::optional<uint32_t>& rslt_;
      };
    }
  }
}

auto ares::database::db::make_char(const uint32_t aid,
                                   const std::string& name,
                                   const uint8_t slot,
                                   const uint16_t head_palette,
                                   const uint16_t head,
                                   const model::pc_job job,
                                   const uint8_t sex,
                                   const uint64_t zeny,
                                   const uint32_t map_id,
                                   const uint16_t map_x,
                                   const uint16_t map_y) -> std::optional<uint32_t> {
  SPDLOG_TRACE(log_, "database::make_char name: '{}'", name);
  std::optional<uint32_t> rslt;
  with_wait_lock([this, &aid, &name, &slot, &head_palette, &head, &job, &sex, &zeny, &map_id, &map_x, &map_y, &rslt] () {
      detail::make_char t(aid, name, slot, head_palette, head, job, sex, zeny, map_id, map_x, map_y, rslt);
      pqxx_conn_->perform(t);
    });

  return rslt;
}

