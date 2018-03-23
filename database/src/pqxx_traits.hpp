#pragma once

#include <chrono>
#include <iomanip>
#include <optional>

#include <ares/model>

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
        return ares::model::to_gmtime_string(*d);
      } else {
        return "";
      }
    }
  };

  template <>
  struct PQXX_LIBEXPORT string_traits<uint8_t> {
    using data_type = uint8_t;

    static const char* name() { return "uint8_t"; };

    static bool has_null() { return false; };

    static bool is_null(const data_type&) { return false; };

    static data_type null() { return data_type(); };

    static void from_string(const char* str, data_type& t) {
      t = std::stoi(str);
    }

    static std::string to_string(const data_type& d) {
      return std::to_string(d);
    }
  };

  template <>
  struct PQXX_LIBEXPORT string_traits<int8_t> {
    using data_type = int8_t;

    static const char* name() { return "int8_t"; };

    static bool has_null() { return false; };

    static bool is_null(const data_type&) { return false; };

    static data_type null() { return data_type(); };

    static void from_string(const char* str, data_type& t) {
      t = std::stoi(str);
    }

    static std::string to_string(const data_type& d) {
      return std::to_string(d);
    }
  };

  template <>
  struct PQXX_LIBEXPORT string_traits<ares::model::account_id> {
    using data_type = ares::model::account_id;

    static const char* name() { return "model::account_id"; };

    static bool has_null() { return false; };

    static bool is_null(const data_type&) { return false; };

    //    static data_type null() { return data_type(); };

    static void from_string(const char* str, data_type& t) {
      t = data_type::from_uint32(std::stoi(str));
    }

    static std::string to_string(const data_type& d) {
      return d.to_string();
    }
  };
  
  template <>
  struct PQXX_LIBEXPORT string_traits<ares::model::character_id> {
    using data_type = ares::model::character_id;

    static const char* name() { return "model::character_id"; };

    static bool has_null() { return false; };

    static bool is_null(const data_type&) { return false; };

    // static data_type null() { return data_type(); };

    static void from_string(const char* str, data_type& t) {
      t = data_type::from_uint32(std::stoi(str));
    }

    static std::string to_string(const data_type& d) {
      return d.to_string();
    }
  };
  
}
