#pragma once

#include <optional>
#include <string>
#include <vector>

#include <ares/model>

namespace ares {
  namespace database {
    namespace record {
      /*! User data record in account server's database
       */
      struct user_data {
        /*! Account id */
        uint32_t aid;
        /*! Account login */
        std::string login;
        /*! Account email */
        std::string email;
        /*! Account level */
        uint32_t level;
        /*! Account sex */
        uint8_t sex;
        /*! Account expiration date/time */
        std::optional<std::chrono::system_clock::time_point> expiration_time;
        /*! Account birthdate */
        std::optional<std::chrono::system_clock::time_point> birthdate;
        /*! Account pin */
        std::optional<std::string> pin;
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
    }
  }
}
