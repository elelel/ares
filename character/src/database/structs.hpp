#pragma once

#include <chrono>
#include <string>
#include <vector>

namespace ares {
  namespace character {
    namespace db {
      namespace record {
        /*! Account data record in character server's database
         */
        struct acc_data {
          uint32_t aid;
          uint8_t normal_slots;
          uint8_t premium_slots;
          uint8_t billing_slots;
          uint8_t creatable_slots;
          uint8_t playable_slots;
          uint32_t bank_vault;
          uint32_t max_storage;
        };
        
        /*! Char data record in character server's database
         */
        struct char_data {
          // TODO: Check Signed -> Unsigned (taken from badly maintained Athena source)
          
          /*! Character id (cid) in SQL database */
          uint32_t cid;
          uint8_t slot;
          std::string name;
          uint8_t sex;
          uint16_t job;
          uint32_t base_level;
          uint32_t job_level;
          uint32_t base_exp;
          uint32_t job_exp;
          int32_t zeny;
          
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
          int32_t effect_state;
          uint8_t virtue;
          int16_t honor;
          
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

          std::string last_map_name;
          uint16_t last_map_x;
          uint16_t last_map_y;

          int8_t rename;
          std::optional<std::chrono::system_clock::time_point> delete_date;
        };
      }
    }
  }
}
