#pragma once

#include <ares/database>
#include <ares/common>

#include "../config.hpp"
#include "structs.hpp"

namespace ares {
  namespace character {
    struct database : ares::database<database> {
      database(std::shared_ptr<spdlog::logger> log,
               const config::postgres_config& conf);

      /*! Create account data record for an aid */
      void account_create(const uint32_t aid,
                          const uint8_t normal_slots,
                          const uint8_t premium_slots,
                          const uint8_t billing_slots,
                          const uint8_t creatable_slots,
                          const uint8_t playable_slots,
                          const uint32_t bank_vault,
                          const uint32_t max_storage);

      /*! Load account slots record for a given aid */
      std::optional<db::record::account_slots> account_slots_for_aid(const uint32_t aid);

      /*! Load account slots record for a given aid */
      std::optional<db::record::account_storage> account_storage_for_aid(const uint32_t aid);


      /*! Create a new character */
      std::optional<uint32_t> char_create(const uint32_t aid,
                                          const uint8_t slot,
                                          const std::string& name,
                                          const uint8_t sex);

      std::optional<size_t> num_chars_for_aid(const uint32_t aid, const size_t max_chars);
      
      std::vector<db::record::character_info> character_info_for_aid(const uint32_t aid, const size_t max_chars);

      std::optional<db::record::character_info> character_info(const uint32_t cid);

      std::optional<db::record::character_info> character_info_for_slot(const uint32_t cid, const uint16_t slot);
      
      std::optional<uint32_t> make_char(const uint32_t aid,
                                        const std::string& name,
                                        const uint8_t slot,
                                        const uint16_t head_palette,
                                        const uint16_t head,
                                        const ares::JOB job,
                                        const uint8_t sex,
                                        const uint64_t zeny,
                                        const std::string& map,
                                        const uint16_t map_x,
                                        const uint16_t map_y);

      std::vector<db::record::map_index> whole_map_index();
    };      
  }
}
