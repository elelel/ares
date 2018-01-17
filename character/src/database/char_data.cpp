#include "database.hpp"

namespace ares {
  namespace character {
    namespace db {
      template <typename QueryResult>
      void assign_result(std::vector<record::char_data>& rslt, QueryResult& qr) {
        rslt.clear();
        for (const auto& sql : qr) {
          rslt.push_back(record::char_data());
          auto& r = rslt[rslt.size() - 1];
          sql["id"].to(r.cid);
          sql["slot"].to(r.slot);
          sql["name"].to(r.name);
          sql["sex"].to(r.sex);
          sql["char_class"].to(r.char_class);
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
          sql["status_point"].to(r.status_point);
          sql["skill_point"].to(r.skill_point);
          sql["option"].to(r.option);
          sql["karma"].to(r.karma);
          sql["manner"].to(r.manner);
          sql["hair"].to(r.hair);
          sql["hair_color"].to(r.hair_color);
          sql["clothes_color"].to(r.clothes_color);
          sql["weapon"].to(r.weapon);
          sql["shield"].to(r.shield);
          sql["head_top"].to(r.head_top);
          sql["head_mid"].to(r.head_mid);
          sql["head_bottom"].to(r.head_bottom);
          sql["robe"].to(r.robe);
          sql["last_map_name"].to(r.last_map_name);
          sql["last_map_x"].to(r.last_map_x);
          sql["last_map_y"].to(r.last_map_y);
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
SELECT "id", "slot", "name", "sex", "char_class", "base_level", "job_level", "base_exp", "job_exp", "zeny",
  "str", "agi", "vit", "int", "dex", "luk", "max_hp", "hp", "max_sp", "sp", "status_point", "skill_point", "option", "karma", "manner",
  "hair", "hair_color", "clothes_color", "weapon", "shield", "head_top", "head_mid", "head_bottom", "robe", "last_map_name", "last_map_x", "last_map_y"
FROM "characters" WHERE ("account_id" = $1) AND ("slot" < 2) LIMIT $2
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