#pragma once

#include <ares/database>

#include "../config.hpp"
#include "structs.hpp"

namespace ares {
  namespace character {
    struct database : ares::database<database> {
      database(std::shared_ptr<spdlog::logger> log,
               const config::postgres_config& conf);

      /*! Create account data record for an aid */
      void acc_data_create(const uint32_t aid,
                           const uint8_t normal_slots,
                           const uint8_t premium_slots,
                           const uint8_t billing_slots,
                           const uint8_t creatable_slots,
                           const uint8_t playable_slots,
                           const uint32_t bank_vault,
                           const uint32_t max_storage);
 
      /*! Load account data record for a given aid */
      std::optional<db::record::acc_data> acc_data_for_aid(const uint32_t aid);

      /*! Create character data, e.g. when user creates a new char */
      void char_data_create(const uint32_t cid,
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
                            const std::optional<std::chrono::system_clock::time_point>& delete_date);
      
      /*! Load vector of char data records for a given aid */
      std::vector<db::record::char_data> char_data_for_aid(const uint32_t aid, const size_t max_chars);
    };
  }
}
