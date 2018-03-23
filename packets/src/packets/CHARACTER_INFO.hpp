struct CHARACTER_INFO {
  inline void emplace(const model::pc_info& pc,
                      const std::string& last_map_name,
                      const int32_t delete_timeout,
                      const uint32_t change_slot_enabled,
                      const uint32_t rename_enabled
                      ) {
    GID_ = model::GID(pc.character_id);
    exp_ = pc.base_exp;
    money_ = pc.zeny;
    jobexp_ = pc.job_exp;
    joblevel_ = pc.job_level;
    bodystate_ = pc.body_state;
    healthstate_ = pc.health_state;
    effectstate_ = pc.effect_state;
    virtue_ = pc.virtue;
    honor_ = pc.honor;
    jobpoint_ = pc.job_point;
    hp_ = pc.hp;
    maxhp_ = pc.max_hp;
    sp_ = pc.sp;
    maxsp_ = pc.max_sp;
    speed_ = pc.speed;
    job_ = pc.job;
    head_ = pc.head;
    body_ = pc.body;
    weapon_ = pc.weapon;
    level_ = pc.base_level;
    sppoint_ = pc.skill_point;
    accessory_ = pc.head_bottom;
    shield_ = pc.shield;
    accessory2_ = pc.head_top;
    accessory3_ = pc.head_mid;
    headpalette_ = pc.head_palette;
    bodypalette_ = pc.head_palette;
    Str_ = pc.Str;
    Agi_ = pc.Agi;
    Vit_ = pc.Vit;
    Int_ = pc.Int;
    Dex_ = pc.Dex;
    Luk_ = pc.Luk;
    CharNum_ = pc.slot;
    rename_ = pc.rename;
    delete_timeout_ = delete_timeout;
    robe_ = pc.robe;
    change_slot_enabled_ = change_slot_enabled;
    rename_enabled_ = rename_enabled;
    sex_ = pc.sex;

    name_ = pc.name;
    last_map_name_.emplace(last_map_name);
  }

  // Workaround for MSVC - wants a default constructor for flexible struct member
  CHARACTER_INFO() {
  }
  
  explicit CHARACTER_INFO(const model::pc_info& pc,
                          const std::string& last_map_name,
                          const int32_t delete_timeout,
                          const uint32_t change_slot_enabled,
                          const uint32_t rename_enabled) {
    emplace(pc, last_map_name, delete_timeout, change_slot_enabled, rename_enabled);
  }
  
private:
  model::GID GID_;
  int64_t exp_;
  int64_t money_;
  int32_t jobexp_;
  int32_t joblevel_;
  int32_t bodystate_;
  int32_t healthstate_;
  int32_t effectstate_;
  int32_t virtue_;
  int32_t honor_;
  int16_t jobpoint_;
  int32_t hp_;
  int32_t maxhp_;
  int16_t sp_;
  int16_t maxsp_;
  int16_t speed_;
  model::pc_job job_;
  int16_t head_;
  int16_t body_;
  int16_t weapon_;
  int16_t level_;
  int16_t sppoint_;
  int16_t accessory_;
  int16_t shield_;
  int16_t accessory2_;
  int16_t accessory3_;
  int16_t headpalette_;
  int16_t bodypalette_;
  model::pc_name_string name_;
  uint8_t Str_;
  uint8_t Agi_;
  uint8_t Vit_;
  uint8_t Int_;
  uint8_t Dex_;
  uint8_t Luk_;
  uint16_t CharNum_;
  uint16_t rename_;
  model::map_name_ext_string last_map_name_;
  int32_t delete_timeout_;
  int32_t robe_;
  uint32_t change_slot_enabled_;
  uint32_t rename_enabled_;
  uint8_t sex_;
};
