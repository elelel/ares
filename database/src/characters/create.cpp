#include "../characters.hpp"

ares::database::characters::create::create(result_type& rslt,
                                           const model::account_id& account_id,
                                           const std::string& name,
                                           const uint8_t& slot,
                                           const uint16_t& head_palette,
                                           const uint16_t& head,
                                           const model::pc_job& job,
                                           const uint8_t& sex,
                                           const uint64_t& zeny,
                                           const uint32_t& map_id,
                                           const uint16_t& map_x,
                                           const uint16_t& map_y) :
  rslt(rslt),
  account_id_(account_id),
  name_(name),
  slot_(slot),
  head_palette_(head_palette),
  head_(head),
  job_(job),
  sex_(sex),
  zeny_(zeny),
  map_id_(map_id),
  map_x_(map_x),
  map_y_(map_y) {
  }

void ares::database::characters::create::operator()(argument_type& trans) {
  // Int, Vit, etc. are first fixed to 1 after modern CH_MAKE_CHAR packet
  const auto Vit = 1;
  const auto Int = 1;
  uint32_t hp = 40 * (100 + Vit) / 100;
  uint32_t sp = 11 * (100 + Int) / 100;
  if (job_ == model::pc_job::SUMMONER) {
    hp = 60 * (100 + Vit) / 100;
    sp = 8 * (100 + Int) / 100;
  }
          
  trans.prepared("character_create")
    (account_id_)(slot_)(name_)(sex_)(static_cast<uint16_t>(job_)).exec();

  auto create_cid = trans.prepared("character_id_by_name")(name_).exec();
          
  if (create_cid.size() == 1) {
    model::character_id cid = model::character_id::from_uint32(0);
    create_cid[0]["id"].to(cid);
            
    trans.prepared("character_create_stats")
      (cid)(hp)(sp).exec();

    trans.prepared("character_create_zeny")
      (cid)(zeny_).exec();
            
    trans.prepared("character_create_appearance")
      (cid)(head_)(head_palette_).exec();

    trans.prepared("character_create_last_location")
      (cid)(map_id_)(map_x_)(map_y_).exec();

    trans.prepared("character_create_save_location")
      (cid)(map_id_)(map_x_)(map_y_).exec();
            
    // TODO: Add items (weapon/armor)

    rslt.emplace(cid);
  } else {
    trans.abort();
  }
}
