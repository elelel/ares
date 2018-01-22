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
          trans.conn().prepare("char_data_for_aid", R"(
SELECT "id", "slot", "name", "sex", "job", "base_level", "job_level", "base_exp", "job_exp", "zeny",
  "str", "agi", "vit", "int", "dex", "luk", "max_hp", "hp", "max_sp", "sp", "job_point", "skill_point", "effect_state", "virtue", "honor",
  "head", "body", "weapon", "robe", "shield", "head_top", "head_mid", "head_bottom", "head_palette", "body_palette", "last_map_name", "last_map_x", "last_map_y", "delete_date", "rename"
FROM "characters" WHERE ("aid" = $1) AND ("slot" < $2) LIMIT $2
)");
          auto qr = trans.prepared("char_data_for_aid")(aid_)(max_chars_).exec();
          assign_result(rslt_, qr);
        }

      private:
        uint32_t aid_;
        size_t max_chars_;
        std::vector<record::char_data>& rslt_;
      };

      struct char_data_create : pqxx::transactor<> {
        char_data_create(const uint32_t& cid,
                         const uint32_t& aid,
                         const uint8_t& slot,
                         const std::string& name,
                         const uint8_t& sex,
                         const uint16_t& job,
                         const uint32_t& base_level,
                         const uint32_t& job_level,
                         const uint32_t& base_exp,
                         const uint32_t& job_exp,
                         const int32_t& zeny,

                         const int8_t& Str,
                         const int8_t& Agi,
                         const int8_t& Vit,
                         const int8_t& Int,
                         const int8_t& Dex,
                         const int8_t& Luk,

                         const int32_t& max_hp,
                         const int32_t& hp,
                         const int32_t& max_sp,
                         const int32_t& sp,
                         const uint8_t& job_point,
                         const uint8_t& skill_point,
                         const int32_t& body_state,
                         const int32_t& health_state,
                         const int32_t& effect_state,
                         const uint8_t& virtue,
                         const int16_t& honor,

                         const int16_t& head,
                         const int16_t& body,
                         const int16_t& weapon,
                         const int16_t& shield,
                         const int16_t& head_top, // accessory2
                         const int16_t& head_mid, // accessory3
                         const int16_t& head_bottom, // accessory
                         const int16_t& head_palette,
                         const int16_t& body_palette,
                         const int32_t& robe,

                         const std::string& last_map_name,
                         const uint16_t& last_map_x,
                         const uint16_t& last_map_y,

                         const int8_t& rename,
                         const std::optional<std::chrono::system_clock::time_point>& delete_date) :
          cid_(cid),
          aid_(aid),
          slot_(slot),
          name_(name),
          sex_(sex),
          job_(job),
          base_level_(base_level),
          job_level_(job_level),
          base_exp_(base_exp),
          job_exp_(job_exp),
          zeny_(zeny),
          Str_(Str),
          Agi_(Agi),
          Vit_(Vit),
          Int_(Int),
          Dex_(Dex),
          Luk_(Luk),
          max_hp_(max_hp),
          hp_(hp),
          max_sp_(max_sp),
          sp_(sp),
          job_point_(job_point),
          skill_point_(skill_point),
          body_state_(body_state),
          health_state_(health_state),
          effect_state_(effect_state),
          virute_(virtue),
          honor_(honor),
          head_(head),
          body_(body),
          weapon_(weapon),
          shield_(shield),
          head_top_(head_top),
          head_mid_(head_mid),
          head_bottom_(head_bottom),
          head_palette_(head_palette),
          body_palette_(body_palette),
          robe_(robe),
          last_map_name_(last_map_name),
          last_map_x_(last_map_x),
          last_map_y_(last_map_y),
          rename_(rename),
          delete_date_(delete_date) {
        }

        void operator()(argument_type& trans) {
          trans.conn().prepare("char_data_create", R"(
INSERT INTO "characters" ("id", "aid", "slot", "name", "sex", "job", "base_level", "job_level", "base_exp", "job_exp", "zeny",
  "str", "agi", "vit", "int", "dex", "luk",
  "max_hp", "hp", "max_sp", "sp", "job_point", "skill_point", "effect_state", "body_state", "health_state", "virtue", "honor",
  "head", "body", "weapon", "shield", "robe", "head_top", "head_mid", "head_bottom", "head_palette", "body_palette",
  "last_map_name", "last_map_x", "last_map_y", "delete_date", "rename")
VALUES ($1, $2, $3, $4, $5, $6, $7, $8, $9, $10, $11, $12
        }
        
      private:
        const uint32_t& cid_;
        const uint32_t& aid_;
        const uint8_t& slot_;
        const std::string& name_;
        const uint8_t& sex_;
        const uint16_t& job_;
        const uint32_t& base_level_;
        const uint32_t& job_level_;
        const uint32_t& base_exp_;
        const uint32_t& job_exp_;
        const int32_t& zeny_;

        const int8_t& Str_;
        const int8_t& Agi_;
        const int8_t& Vit_;
        const int8_t& Int_;
        const int8_t& Dex_;
        const int8_t& Luk_;

        const int32_t& max_hp_;
        const int32_t& hp_;
        const int32_t& max_sp_;
        const int32_t& sp_;
        const uint8_t& job_point_;
        const uint8_t& skill_point_;
        const int32_t& body_state_;
        const int32_t& health_state_;
        const int32_t& effect_state_;
        const uint8_t& virtue_;
        const int16_t& honor_;

        const int16_t& head_;
        const int16_t& body_;
        const int16_t& weapon_;
        const int16_t& shield_;
        const int16_t& head_top_; // accessory2
        const int16_t& head_mid_; // accessory3
        const int16_t& head_bottom_; // accessory
        const int16_t& head_palette_;
        const int16_t& body_palette_;
        const int32_t& robe_;

        const std::string& last_map_name_;
        const uint16_t& last_map_x_;
        const uint16_t& last_map_y_;

        const int8_t& rename_;
        const std::optional<std::chrono::system_clock::time_point>& delete_date_;
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

  std::vector<db::record::char_data> rslt;
  with_wait_lock([this, &cid, &aid, &slot, &name, &sex, &job, &base_level, &job_level, &base_exp, &job_exp, &zeny,
                  &Str, &Agi, &Vit, &Int, &Dex, &Luk,
                  &max_hp, &hp, &max_sp, &sp, &job_point, &skill_point, &body_state, &health_state, &effect_state, &virtue, &honor,
                  &head, &body, &weapon, &shield, &head_top, &head_mid, &head_bottom, &head_palette, &body_palette, &robe,
                  &last_map_name, &last_map_x, &last_map_y, &rename, &delete_date] () {
      db::char_data_create t(cid, aid, slot, name, sex, job, base_level, job_level, base_exp, job_exp, zeny,
                             Str, Agi, Vit, Int, Dex, Luk,
                             max_hp, hp, max_sp, sp, job_point, skill_point, body_state, health_state, effect_state, virtue, honor,
                             head, body, weapon, shield, head_top, head_mid, head_bottom, head_palette, body_palette, robe,
                             last_map_name, last_map_x, last_map_y, rename, &delete_date);
      pqxx_conn_->perform(t);
    });
}

