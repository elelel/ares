#pragma once

#include <optional>
#include <string>

namespace ares {
  namespace account {
    namespace db {
      namespace record {
        struct user_data {
          uint32_t aid;
          std::string login;
          std::string email;
          uint32_t level;
          uint8_t sex;
          std::optional<std::chrono::system_clock::time_point> expiration_time;
          std::optional<std::chrono::system_clock::time_point> birthdate;
          std::optional<std::string> pin;
        };
      }
    }
  }
}
