#pragma once

#include <chrono>
#include <iomanip>
#include <optional>

namespace pqxx {
  /*! Custom trait for libpqxx to seamlessly perform SQL operation on time_point datetimes */
  template <>
  struct PQXX_LIBEXPORT string_traits<std::optional<std::chrono::system_clock::time_point>> {
    using data_type = std::optional<std::chrono::system_clock::time_point>;
    
    static const char* name() { return "std::chrono::system_clock::time_point"; };
    
    static bool has_null() { return true; };
    
    static bool is_null(const data_type& r) { return !r; };
    
    static data_type null() { return data_type(); };
    
    static void from_string(const char* str, data_type& t) {
      std::tm tm = {};
      std::stringstream ss(str);
      ss >> std::get_time(&tm, "%Y-%m-%d %H:%M:%S");
      if (ss.fail()) {
        std::stringstream ss(str);
        ss >> std::get_time(&tm, "%Y-%m-%d");
        if (ss.fail()) throw std::runtime_error("Failed to convert PostgreSQL time string '" + std::string(str) + "' to " + name());
      }
      std::time_t tt = std::mktime(&tm);
      t = std::chrono::system_clock::from_time_t(tt);
    }

    static std::string to_string(const data_type& d) {
      if (d) {
        auto tt = std::chrono::system_clock::to_time_t(*d);
        auto tm = gmtime(&tt);
        std::stringstream ss;
        ss << std::put_time(tm, "%Y-%m-%d %H:%M:%S");
        if (!ss.fail()) {
          return ss.str();
        } else {
          throw std::runtime_error("Failed to format time string for PostgreSQL");
        }
      } else {
        return "";
      }
    }
  };

}
