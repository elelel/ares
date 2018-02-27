#include "../database.hpp"

namespace ares {
  namespace database {
    namespace detail {
      template <typename SqlRecord>
      inline void assign(model::pc_info& pc, const SqlRecord& sql) {
        sql["id"].to(pc.cid);
        std::string tmp_str;
        sql["name"].to(tmp_str);
        pc.name = tmp_str;
        sql["sex"].to(pc.sex);
        uint16_t tmp_uint16;
        sql["job"].to(tmp_uint16);
        pc.job = ares::model::pc_job{tmp_uint16};

        sql["slot"].to(pc.slot);
        sql["rename"].to(pc.rename);
          
        sql["base_level"].to(pc.base_level);
        sql["job_level"].to(pc.job_level);
        sql["base_exp"].to(pc.base_exp);
        sql["job_exp"].to(pc.job_exp);
        sql["zeny"].to(pc.zeny);
          
        sql["str"].to(pc.Str);
        sql["agi"].to(pc.Agi);
        sql["vit"].to(pc.Vit);
        sql["int"].to(pc.Int);
        sql["dex"].to(pc.Dex);
        sql["luk"].to(pc.Luk);
          
        sql["max_hp"].to(pc.max_hp);
        sql["hp"].to(pc.hp);
        sql["max_sp"].to(pc.max_sp);
        sql["sp"].to(pc.sp);
        sql["job_point"].to(pc.job_point);
        sql["skill_point"].to(pc.skill_point);
        sql["effect_state"].to(pc.effect_state);
        sql["body_state"].to(pc.body_state);
        sql["health_state"].to(pc.health_state);
        sql["virtue"].to(pc.virtue);
        sql["honor"].to(pc.honor);
          
        sql["head"].to(pc.head);
        sql["body"].to(pc.body);
        sql["weapon"].to(pc.weapon);
        sql["robe"].to(pc.robe);          
        sql["shield"].to(pc.shield);
        sql["head_top"].to(pc.head_top);
        sql["head_mid"].to(pc.head_mid);
        sql["head_bottom"].to(pc.head_bottom);
        sql["head_palette"].to(pc.head_palette);
        sql["body_palette"].to(pc.body_palette);

        sql["save_map_id"].to(pc.location_save.map_id);
        sql["save_x"].to(pc.location_save.coords.x);
        sql["save_y"].to(pc.location_save.coords.y);
        sql["last_map_id"].to(pc.location_last.map_id);
        sql["last_x"].to(pc.location_last.coords.x);
        sql["last_y"].to(pc.location_last.coords.y);

        // weapon can't be != 0 if the option is 'riding'
        if (pc.effect_state & 0x7e80020) { pc.weapon = 0; }
      }
      
      struct character_info_for_aid : pqxx::transactor<> {
        character_info_for_aid(const uint32_t& aid, const size_t& max_chars, std::vector<model::pc_info>& rslt) :
          aid_(aid),
          max_chars_(max_chars),
          rslt_(rslt) {
        }

        void operator()(argument_type& trans) {
          rslt_.clear();
          auto qr = trans.prepared("character_info_for_aid")(aid_)(max_chars_).exec();
          for (const auto& sql : qr) {
            rslt_.push_back(model::pc_info());
            assign(rslt_[rslt_.size() - 1], sql);
          }
        }

      private:
        uint32_t aid_;
        size_t max_chars_;
        std::vector<model::pc_info>& rslt_;
      };

      struct character_info : pqxx::transactor<> {
        character_info(const uint32_t& cid, std::optional<model::pc_info>& rslt) :
          cid_(cid),
          rslt_(rslt) {
        }

        void operator()(argument_type& trans) {
          rslt_.reset();
          auto qr = trans.prepared("character_info")(cid_).exec();
          if (qr.size() == 1) {
            model::pc_info r;
            assign(r, qr[0]);
            rslt_.emplace(std::move(r));
          }
        }

      private:
        uint32_t cid_;
        std::optional<model::pc_info>& rslt_;
      };
      
      struct character_info_for_slot : pqxx::transactor<> {
        character_info_for_slot(const uint32_t aid, const uint16_t slot, std::optional<model::pc_info>& rslt) :
          aid_(aid),
          slot_(slot),
          rslt_(rslt) {
        }

        void operator()(argument_type& trans) {
          rslt_.reset();
          auto qr = trans.prepared("character_info_for_slot")(aid_)(slot_).exec();
          if (qr.size() == 1) {
            model::pc_info r;
            assign(r, qr[0]);
            rslt_.emplace(std::move(r));
          }
        }
        
      private:
        uint32_t aid_;
        uint16_t slot_;
        std::optional<model::pc_info>& rslt_;
      };
      
    }
  }
}

auto ares::database::db::character_info_for_aid(const uint32_t aid, const size_t max_chars) -> std::vector<model::pc_info> {
  std::vector<model::pc_info> rslt;
  with_wait_lock([this, aid, max_chars, &rslt] () {
      detail::character_info_for_aid t(aid, max_chars, rslt);
      pqxx_conn_->perform(t);
    });
  return rslt;
}

auto ares::database::db::character_info(const uint32_t cid) -> std::optional<model::pc_info> {
  std::optional<model::pc_info> rslt;
  with_wait_lock([this, cid, &rslt] () {
      detail::character_info t(cid, rslt);
      pqxx_conn_->perform(t);
    });
  return rslt;
}

auto ares::database::db::character_info_for_slot(const uint32_t aid, const uint16_t slot) -> std::optional<model::pc_info> {
  std::optional<model::pc_info> rslt;
  with_wait_lock([this, aid, slot, &rslt] () {
      detail::character_info_for_slot t(aid, slot, rslt);
      pqxx_conn_->perform(t);
    });
  return rslt;
}
