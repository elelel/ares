#pragma once

#include <optional>
#include <string>

namespace ares {
  namespace account {
    namespace db {
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
      }
    }
  }
}
