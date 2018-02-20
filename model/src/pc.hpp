#pragma once

namespace ares {
  namespace model {
    struct pc_name_string : fixed_string<24> {
      using fixed_string<24>::fixed_string;
    };

    /*! Character info, should contain only semi-POD structures
     */
    struct pc_info {
      uint32_t cid;
      uint32_t aid;
      pc_name_string name;
      uint8_t sex;
      uint16_t job;
      uint8_t slot;
      int8_t rename;

      int16_t head;
      int16_t body;
      int16_t weapon;
      int16_t shield;
      int16_t head_top; // accessory2
      int16_t head_mid; // accessory3
      int16_t head_bottom; // accessory
      int16_t head_palette;
      int16_t body_palette;
      int32_t robe;
 
      uint32_t base_level;
      uint32_t job_level;
      uint64_t base_exp;
      uint32_t job_exp;
      uint64_t zeny;
      int16_t speed;
          
      int8_t Str;
      int8_t Agi;
      int8_t Vit;
      int8_t Int;
      int8_t Dex;
      int8_t Luk;
          
      int32_t max_hp;
      int32_t hp;
      int32_t max_sp;
      int32_t sp;
      uint8_t job_point;
      uint8_t skill_point;
      int32_t body_state;
      int32_t health_state;
      int32_t effect_state;  // 'option'
      uint8_t virtue;
      int16_t honor;
 
      map_location location_last;
      map_location location_save;
    };
  }
}
