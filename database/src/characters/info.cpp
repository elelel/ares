#include "../characters.hpp"

template <typename SqlRecord>
inline void assign(ares::model::pc_info& pc, const SqlRecord& sql) {
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

ares::database::characters::info::info(result_type& rslt, const uint32_t& id) :
  rslt(rslt),
  id_(id) {
}

void ares::database::characters::info::operator()(argument_type& trans) {
  auto qr = trans.prepared("character_info")(id_).exec();
  if (qr.size() == 1) {
    model::pc_info r;
    assign(r, qr[0]);
    rslt.emplace(std::move(r));
  }
}

ares::database::characters::infos_for_aid::infos_for_aid(result_type& rslt, const uint32_t& aid, const size_t& max_chars) :
  rslt(rslt),
  aid_(aid),
  max_chars_(max_chars) {
  }

void ares::database::characters::infos_for_aid::operator()(argument_type& trans) {
  auto qr = trans.prepared("character_infos_for_aid")(aid_)(max_chars_).exec();
  for (const auto& sql : qr) {
    model::pc_info r;
    assign(r, sql);
    rslt.push_back(std::move(r));
  }
}

ares::database::characters::info_for_slot::info_for_slot(result_type& rslt, const uint32_t& aid, const uint16_t& slot) :
  rslt(rslt),
  aid_(aid),
  slot_(slot) {
  }

void ares::database::characters::info_for_slot::operator()(argument_type& trans) {
  auto qr = trans.prepared("character_info_for_slot")(aid_)(slot_).exec();
  if (qr.size() == 1) {
    model::pc_info r;
    assign(r, qr[0]);
    rslt.emplace(std::move(r));
  }
}
