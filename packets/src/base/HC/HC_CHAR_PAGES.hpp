struct HC_CHAR_PAGES {
  inline void emplace(const size_t nchars) {
    PacketType = packet_id::HC_CHAR_PAGES;
    PacketLength = sizeof(*this) +  nchars * sizeof(CHARACTER_INFO);
  }

  explicit HC_CHAR_PAGES(const size_t nchars) {
    emplace(nchars);
  }

  struct CHARACTER_INFO {
    inline void emplace(const uint32_t GID,
                        const int32_t exp,
                        const int32_t money,
                        const int32_t jobexp,
                        const int32_t joblevel,
                        const int32_t bodystate,
                        const int32_t healthstate,
                        const int32_t effectstate,
                        const int32_t virtue,
                        const int32_t honor,
                        const int16_t jobpoint,
                        const int32_t hp,
                        const int32_t maxhp,
                        const int16_t sp,
                        const int16_t maxsp,
                        const int16_t speed,
                        const int16_t job,
                        const int16_t head,
                        const int16_t body,
                        const int16_t weapon,
                        const int16_t level,
                        const int16_t sppoint,
                        const int16_t accessory,
                        const int16_t shield,
                        const int16_t accessory2,
                        const int16_t accessory3,
                        const int16_t headpalette,
                        const int16_t bodypalette,
                        const char* name,
                        const size_t name_sz,
                        const uint8_t Str,
                        const uint8_t Agi,
                        const uint8_t Vit,
                        const uint8_t Int,
                        const uint8_t Dex,
                        const uint8_t Luk,
                        const uint8_t CharNum,
                        const uint8_t haircolor,
                        const uint8_t rename,
                        const char* last_map_name,
                        const size_t last_map_name_sz,
                        const int32_t delete_timeout,
                        const int32_t robe,
                        const uint32_t change_slot_enabled,
                        const uint32_t rename_enabled,
                        const uint8_t sex) {
      GID_ = GID;
      exp_ = exp;
      money_ = money;
      jobexp_ = jobexp;
      joblevel_ = joblevel;
      bodystate_ = bodystate;
      healthstate_ = healthstate;
      effectstate_ = effectstate;
      virtue_ = virtue;
      honor_ = honor;
      jobpoint_ = jobpoint;
      hp_ = hp;
      maxhp_ = maxhp;
      sp_ = sp;
      maxsp_ = maxsp;
      speed_ = speed;
      job_ = job;
      head_ = head;
      body_ = body;
      weapon_ = weapon;
      level_ = level;
      sppoint_ = sppoint;
      accessory_ = accessory;
      shield_ = shield;
      accessory2_ = accessory2;
      accessory3_ = accessory3;
      headpalette_ = headpalette;
      bodypalette_ = bodypalette;
      Str_ = Str;
      Agi_ = Agi;
      Vit_ = Vit;
      Int_ = Int;
      Dex_ = Dex;
      Luk_ = Luk;
      CharNum_ = CharNum;
      rename_ = rename;
      haircolor_ = haircolor;
      delete_timeout_ = delete_timeout;
      robe_ = robe;
      change_slot_enabled_ = change_slot_enabled;
      rename_enabled_ = rename_enabled;
      haircolor_ = haircolor;
      sex_ = sex;

      copy_buf_with_zero_pad(name_, sizeof(name_), name, name_sz);
      copy_buf_with_zero_pad(last_map_name_, sizeof(last_map_name_), last_map_name, last_map_name_sz);
    }

    inline void emplace(const uint32_t GID,
                        const int32_t exp,
                        const int32_t money,
                        const int32_t jobexp,
                        const int32_t joblevel,
                        const int32_t bodystate,
                        const int32_t healthstate,
                        const int32_t effectstate,
                        const int32_t virtue,
                        const int32_t honor,
                        const int16_t jobpoint,
                        const int32_t hp,
                        const int32_t maxhp,
                        const int16_t sp,
                        const int16_t maxsp,
                        const int16_t speed,
                        const int16_t job,
                        const int16_t head,
                        const int16_t body,
                        const int16_t weapon,
                        const int16_t level,
                        const int16_t sppoint,
                        const int16_t accessory,
                        const int16_t shield,
                        const int16_t accessory2,
                        const int16_t accessory3,
                        const int16_t headpalette,
                        const int16_t bodypalette,
                        const std::string&  name,
                        const uint8_t Str,
                        const uint8_t Agi,
                        const uint8_t Vit,
                        const uint8_t Int,
                        const uint8_t Dex,
                        const uint8_t Luk,
                        const uint8_t CharNum,
                        const uint8_t haircolor,
                        const uint8_t rename,
                        const std::string& last_map_name,
                        const int32_t delete_timeout,
                        const int32_t robe,
                        const uint32_t change_slot_enabled,
                        const uint32_t rename_enabled,
                        const uint8_t sex) {
      emplace(GID, exp, money, jobexp, joblevel, bodystate, healthstate, effectstate, virtue, honor, jobpoint, hp, maxhp, sp, maxsp, speed,
              job, head, body, weapon, level, sppoint, accessory, shield, accessory2, accessory3, headpalette, bodypalette, name,
              Str, Agi,Vit, Int, Dex, Luk, CharNum, haircolor, rename, last_map_name, delete_timeout, robe,
              change_slot_enabled, rename_enabled, sex);
    }

    explicit CHARACTER_INFO(const uint32_t GID,
                            const int32_t exp,
                            const int32_t money,
                            const int32_t jobexp,
                            const int32_t joblevel,
                            const int32_t bodystate,
                            const int32_t healthstate,
                            const int32_t effectstate,
                            const int32_t virtue,
                            const int32_t honor,
                            const int16_t jobpoint,
                            const int32_t hp,
                            const int32_t maxhp,
                            const int16_t sp,
                            const int16_t maxsp,
                            const int16_t speed,
                            const int16_t job,
                            const int16_t head,
                            const int16_t body,
                            const int16_t weapon,
                            const int16_t level,
                            const int16_t sppoint,
                            const int16_t accessory,
                            const int16_t shield,
                            const int16_t accessory2,
                            const int16_t accessory3,
                            const int16_t headpalette,
                            const int16_t bodypalette,
                            const char*  name,
                            const size_t name_sz,
                            const uint8_t Str,
                            const uint8_t Agi,
                            const uint8_t Vit,
                            const uint8_t Int,
                            const uint8_t Dex,
                            const uint8_t Luk,
                            const uint8_t CharNum,
                            const uint8_t haircolor,
                            const uint8_t rename,
                            const char* last_map_name,
                            const size_t last_map_name_sz,
                            const int32_t delete_timeout,
                            const int32_t robe,
                            const uint32_t change_slot_enabled,
                            const uint32_t rename_enabled,
                            const uint8_t sex) {
      emplace(GID, exp, money, jobexp, joblevel, bodystate, healthstate, effectstate, virtue, honor, jobpoint, hp, maxhp, sp, maxsp, speed,
              job, head, body, weapon, level, sppoint, accessory, shield, accessory2, accessory3, headpalette, bodypalette, name, name_sz,
              Str, Agi,Vit, Int, Dex, Luk, CharNum, haircolor, rename, last_map_name, last_map_name_sz, delete_timeout, robe,
              change_slot_enabled, rename_enabled, sex);
    }

    explicit CHARACTER_INFO(const uint32_t GID,
                            const int32_t exp,
                            const int32_t money,
                            const int32_t jobexp,
                            const int32_t joblevel,
                            const int32_t bodystate,
                            const int32_t healthstate,
                            const int32_t effectstate,
                            const int32_t virtue,
                            const int32_t honor,
                            const int16_t jobpoint,
                            const int32_t hp,
                            const int32_t maxhp,
                            const int16_t sp,
                            const int16_t maxsp,
                            const int16_t speed,
                            const int16_t job,
                            const int16_t head,
                            const int16_t body,
                            const int16_t weapon,
                            const int16_t level,
                            const int16_t sppoint,
                            const int16_t accessory,
                            const int16_t shield,
                            const int16_t accessory2,
                            const int16_t accessory3,
                            const int16_t headpalette,
                            const int16_t bodypalette,
                            const std::string&  name,
                            const uint8_t Str,
                            const uint8_t Agi,
                            const uint8_t Vit,
                            const uint8_t Int,
                            const uint8_t Dex,
                            const uint8_t Luk,
                            const uint8_t CharNum,
                            const uint8_t haircolor,
                            const uint8_t rename,
                            const std::string& last_map_name,
                            const int32_t delete_timeout,
                            const int32_t robe,
                            const uint32_t change_slot_enabled,
                            const uint32_t rename_enabled,
                            const uint8_t sex) {
      emplace(GID, exp, money, jobexp, joblevel, bodystate, healthstate, effectstate, virtue, honor, jobpoint, hp, maxhp, sp, maxsp, speed,
              job, head, body, weapon, level, sppoint, accessory, shield, accessory2, accessory3, headpalette, bodypalette, name,
              Str, Agi,Vit, Int, Dex, Luk, CharNum, haircolor, rename, last_map_name, delete_timeout, robe,
              change_slot_enabled, rename_enabled, sex);
    }
  
  private:
    uint32_t GID_;
    int32_t exp_;
    int32_t money_;
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
    int16_t job_;
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
    char name_[24];
    uint8_t Str_;
    uint8_t Agi_;
    uint8_t Vit_;
    uint8_t Int_;
    uint8_t Dex_;
    uint8_t Luk_;
    uint8_t CharNum_;
    uint8_t haircolor_;
    uint8_t rename_;
    char last_map_name_[16];
    int32_t delete_timeout_;
    int32_t robe_;
    uint32_t change_slot_enabled_;
    uint32_t rename_enabled_;
    uint8_t sex_;
  };

  uint16_t PacketType;
  uint16_t PacketLength;

  const CHARACTER_INFO* characters() const {
    return characters_;
  }
  
private:
  CHARACTER_INFO characters_[];
};
