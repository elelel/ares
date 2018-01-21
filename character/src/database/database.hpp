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

      
      /*! Load vector of char data records for a given aid */
      std::vector<db::record::char_data> char_data_for_aid(const uint32_t aid, const size_t max_chars);
    };
  }
}
