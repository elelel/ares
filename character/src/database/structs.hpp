#pragma once

#include <chrono>
#include <string>
#include <vector>

#include <ares/model>

namespace ares {
  namespace character {
    namespace db {
      namespace record {
        struct map_index {
          uint32_t id;
          uint32_t external_id;
          std::string name;
        };

        /*! Account slots info in character server's database
         */
        struct account_slots {
          uint32_t aid;
          uint8_t normal_slots;
          uint8_t premium_slots;
          uint8_t billing_slots;
          uint8_t creatable_slots;
          uint8_t playable_slots;
        };

        /*! Account storage info in character server's database
         */
        struct account_storage {
          uint32_t bank_vault;
          uint32_t max_storage;
        };

        struct character_info {
          model::pc_info pc;
          std::optional<std::chrono::system_clock::time_point> delete_date;
        };

      }
    }
  }
}
