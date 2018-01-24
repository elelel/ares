#include "database.hpp"

namespace ares {
  namespace character {
    namespace db {
      template <typename SqlRecord>
      inline record::character_info& assign(record::character_info& char_info, const SqlRecord& sql) {
        auto& i = char_info.info;
        auto& a = char_info.appearance;
        auto& s = char_info.stats;
        auto& l = char_info.location;

        sql["id"].to(i.cid);
        sql["slot"].to(i.slot);
        sql["name"].to(i.name);
        sql["sex"].to(i.sex);
        sql["job"].to(i.job);
        sql["delete_date"].to(i.delete_date);
        sql["rename"].to(i.rename);
          
        sql["base_level"].to(s.base_level);
        sql["job_level"].to(s.job_level);
        sql["base_exp"].to(s.base_exp);
        sql["job_exp"].to(s.job_exp);
        sql["zeny"].to(s.zeny);
          
        sql["str"].to(s.Str);
        sql["agi"].to(s.Agi);
        sql["vit"].to(s.Vit);
        sql["int"].to(s.Int);
        sql["dex"].to(s.Dex);
        sql["luk"].to(s.Luk);
          
        sql["max_hp"].to(s.max_hp);
        sql["hp"].to(s.hp);
        sql["max_sp"].to(s.max_sp);
        sql["sp"].to(s.sp);
        sql["job_point"].to(s.job_point);
        sql["skill_point"].to(s.skill_point);
        sql["effect_state"].to(s.effect_state);
        sql["body_state"].to(s.body_state);
        sql["health_state"].to(s.health_state);
        sql["virtue"].to(s.virtue);
        sql["honor"].to(s.honor);
          
        sql["head"].to(a.head);
        sql["body"].to(a.body);
        sql["weapon"].to(a.weapon);
        sql["robe"].to(a.robe);          
        sql["shield"].to(a.shield);
        sql["head_top"].to(a.head_top);
        sql["head_mid"].to(a.head_mid);
        sql["head_bottom"].to(a.head_bottom);
        sql["head_palette"].to(a.head_palette);
        sql["body_palette"].to(a.body_palette);

        sql["map_name"].to(l.map_name);
        sql["map_x"].to(l.map_x);
        sql["map_y"].to(l.map_y);

        // weapon can't be != 0 if the option is 'riding'
        if (s.effect_state & 0x7e80020) { a.weapon = 0; }
        
        return char_info;
      }
      
      struct character_info_for_aid : pqxx::transactor<> {
        character_info_for_aid(const uint32_t& aid, const size_t& max_chars, std::vector<record::character_info>& rslt) :
          aid_(aid),
          max_chars_(max_chars),
          rslt_(rslt) {
        }

        void operator()(argument_type& trans) {
          rslt_.clear();
          auto qr = trans.prepared("character_info_for_aid")(aid_)(max_chars_).exec();
          for (const auto& sql : qr) {
            rslt_.push_back(record::character_info());
            assign(rslt_[rslt_.size() - 1], sql);
          }
        }

      private:
        uint32_t aid_;
        size_t max_chars_;
        std::vector<record::character_info>& rslt_;
      };

      struct character_info : pqxx::transactor<> {
        character_info(const uint32_t& cid, std::optional<record::character_info>& rslt) :
          cid_(cid),
          rslt_(rslt) {
        }

        void operator()(argument_type& trans) {
          rslt_.reset();
          auto qr = trans.prepared("character_info")(cid_).exec();
          if (qr.size() > 0) {
            record::character_info r;
            assign(r, qr[0]);
            rslt_.emplace(std::move(r));
          }
        }

      private:
        uint32_t cid_;
        std::optional<record::character_info>& rslt_;
      };
      
      struct character_info_for_slot : pqxx::transactor<> {
        character_info_for_slot(const uint32_t cid, const uint16_t slot, std::optional<record::character_info>& rslt) :
          cid_(cid),
          slot_(slot),
          rslt_(rslt) {
        }

        void operator()(argument_type& trans) {
          rslt_.reset();
          auto qr = trans.prepared("character_info_for_slot")(cid_)(slot_).exec();
          if (qr.size() > 0) {
            record::character_info r;
            assign(r, qr[0]);
            rslt_.emplace(std::move(r));
          }
        }
        
      private:
        uint32_t cid_;
        uint16_t slot_;
        std::optional<record::character_info>& rslt_;
      };
        
      
    }
  }
}

auto ares::character::database::character_info_for_aid(const uint32_t aid, const size_t max_chars) -> std::vector<db::record::character_info> {
  std::vector<db::record::character_info> rslt;
  with_wait_lock([this, &aid,  &max_chars, &rslt] () {
      db::character_info_for_aid t(aid, max_chars, rslt);
      pqxx_conn_->perform(t);
    });
  return rslt;
}


auto ares::character::database::character_info(const uint32_t cid) -> std::optional<db::record::character_info> {
  std::optional<db::record::character_info> rslt;
  with_wait_lock([this, &cid, &rslt] () {
      db::character_info t(cid, rslt);
      pqxx_conn_->perform(t);
    });
  return rslt;
}

auto ares::character::database::character_info_for_slot(const uint32_t cid, const uint16_t slot) -> std::optional<db::record::character_info> {
  std::optional<db::record::character_info> rslt;
  with_wait_lock([this, &cid, slot, &rslt] () {
      db::character_info_for_slot t(cid, slot, rslt);
      pqxx_conn_->perform(t);
    });
  return rslt;
}
  
