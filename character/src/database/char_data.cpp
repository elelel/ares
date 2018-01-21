#include "database.hpp"

namespace ares {
  namespace character {
    namespace db {
      template <typename QueryResult>
      inline void assign_result(std::vector<record::char_data>& rslt, QueryResult& qr) {
        rslt.clear();
        for (const auto& sql : qr) {
          rslt.push_back(record::char_data());
          auto& r = rslt[rslt.size() - 1];
          sql["id"].to(r.cid);
          sql["slot"].to(r.slot);
          sql["name"].to(r.name);
          sql["sex"].to(r.sex);
          sql["job"].to(r.job);
          sql["base_level"].to(r.base_level);
          sql["job_level"].to(r.job_level);
          sql["base_exp"].to(r.base_exp);
          sql["job_exp"].to(r.job_exp);
          sql["zeny"].to(r.zeny);
          
          sql["str"].to(r.Str);
          sql["agi"].to(r.Agi);
          sql["vit"].to(r.Vit);
          sql["int"].to(r.Int);
          sql["dex"].to(r.Dex);
          sql["luk"].to(r.Luk);
          
          sql["max_hp"].to(r.max_hp);
          sql["hp"].to(r.hp);
          sql["max_sp"].to(r.max_sp);
          sql["sp"].to(r.sp);
          sql["job_point"].to(r.job_point);
          sql["skill_point"].to(r.skill_point);
          sql["effect_state"].to(r.effect_state);
          sql["body_state"].to(r.body_state);
          sql["health_state"].to(r.health_state);
          sql["virtue"].to(r.virtue);
          sql["honor"].to(r.honor);
          
          sql["head"].to(r.head);
          sql["body"].to(r.body);
          sql["weapon"].to(r.weapon);
          sql["robe"].to(r.robe);          
          sql["shield"].to(r.shield);
          sql["head_top"].to(r.head_top);
          sql["head_mid"].to(r.head_mid);
          sql["head_bottom"].to(r.head_bottom);
          sql["head_palette"].to(r.head_palette);
          sql["body_palette"].to(r.body_palette);

          sql["last_map_name"].to(r.last_map_name);
          sql["last_map_x"].to(r.last_map_x);
          sql["last_map_y"].to(r.last_map_y);
          sql["delete_date"].to(r.delete_date);
          sql["rename"].to(r.rename);

          // weapon can't be != 0 if the option is 'riding'
          if (r.effect_state & 0x7e80020) {
            r.weapon = 0;
          }
        }
      }

      struct char_data_for_aid : pqxx::transactor<> {
        char_data_for_aid(const uint32_t aid, const size_t max_chars, std::vector<record::char_data>& rslt) :
          aid_(aid),
          max_chars_(max_chars),
          rslt_(rslt) {
        }

        void operator()(argument_type& trans) {
          trans.conn().prepare("char_data", R"(
SELECT "id", "slot", "name", "sex", "job", "base_level", "job_level", "base_exp", "job_exp", "zeny",
  "str", "agi", "vit", "int", "dex", "luk", "max_hp", "hp", "max_sp", "sp", "job_point", "skill_point", "effect_state", "virtue", "honor",
  "head", "body", "weapon", "robe", "shield", "head_top", "head_mid", "head_bottom", "head_palette", "body_palette", "last_map_name", "last_map_x", "last_map_y", "delete_date", "rename"
FROM "characters" WHERE ("aid" = $1) AND ("slot" < $2) LIMIT $2
)");
          auto qr = trans.prepared("char_data")(aid_)(max_chars_).exec();
          assign_result(rslt_, qr);
        }

      private:
        uint32_t aid_;
        size_t max_chars_;
        std::vector<record::char_data>& rslt_;
      };
    }
  }
}

auto ares::character::database::char_data_for_aid(const uint32_t aid, const size_t max_chars) -> std::vector<db::record::char_data> {
  std::vector<db::record::char_data> rslt;
  with_wait_lock([this, &aid, &rslt, &max_chars] () {
      db::char_data_for_aid t(aid, max_chars, rslt);
      pqxx_conn_->perform(t);
    });
  return rslt;
}

/*
void ares::character::database::char_data_create(const uint32_t cid,
                                                 const uint32_t aid,
                                                 const uint8_t slot,
                                                 const std::string& name,
                                                 const uint8_t sex,
                                                 const uint16_t job,
                                                 const uint32_t base_level,
                                                 const uint32_t job_level,
                                                 const uint32_t base_exp,
                                                 const uint32_t job_exp,
                                                 const int32_t zeny,

                                                 const int8_t Str,
                                                 const int8_t Agi,
                                                 const int8_t Vit,
                                                 const int8_t Int,
                                                 const int8_t Dex,
                                                 const int8_t Luk,

                                                 const int32_t max_hp,
                                                 const int32_t hp,
                                                 const int32_t max_sp,
                                                 const int32_t sp,
                                                 const uint8_t job_point,
                                                 const uint8_t skill_point,
                                                 const int32_t body_state,
                                                 const int32_t health_state,
                                                 const int32_t effect_state,
                                                 const uint8_t virtue,
                                                 const int16_t honor,

                                                 const int16_t head,
                                                 const int16_t body,
                                                 const int16_t weapon,
                                                 const int16_t shield,
                                                 const int16_t head_top, // accessory2
                                                 const int16_t head_mid, // accessory3
                                                 const int16_t head_bottom, // accessory
                                                 const int16_t head_palette,
                                                 const int16_t body_palette,
                                                 const int32_t robe,

                                                 const std::string& last_map_name,
                                                 const uint16_t last_map_x,
                                                 const uint16_t last_map_y,

                                                 const int8_t rename,
                                                 const std::optional<std::chrono::system_clock::time_point>& delete_date
                                                 ) {

}
*/
