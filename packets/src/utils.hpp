#pragma once

#include <sstream>
#include <chrono>
#include <iomanip>

namespace ares {
  inline static void copy_buf_with_zero_pad(void* dest, const size_t dest_sz, const void* str, const size_t src_sz) {
    auto len = src_sz;
    if (src_sz > dest_sz) len = dest_sz;
    memcpy(dest, str, len);
    memset((void*)((uintptr_t)dest + len), 0, dest_sz - len);
  }

  inline static uint32_t to_host_epoch_time(const std::chrono::system_clock::time_point& time_point) {
    return std::chrono::duration_cast<std::chrono::seconds>(time_point.time_since_epoch()).count();
  }

  inline static std::string to_gmtime_string_fmt(const std::chrono::system_clock::time_point& time_point,
                                                  const char* fmt) {
    auto tt = std::chrono::system_clock::to_time_t(time_point);
    auto tm = gmtime(&tt);
    std::stringstream ss;
    ss << std::put_time(tm, fmt);
    if (!ss.fail()) {
      return ss.str();
    } else {
      throw std::runtime_error("Failed to format gmtime");
    }
  }

  inline static std::string to_gmtime_string(const std::chrono::system_clock::time_point& time_point) {
    return to_gmtime_string_fmt(time_point, "%Y-%m-%d %H:%M:%S");
  }

  inline static std::string to_gmdate_string(const std::chrono::system_clock::time_point& time_point) {
    return to_gmtime_string_fmt(time_point, "%Y-%m-%d");
  }

  inline static std::string to_localtime_string_fmt(const std::chrono::system_clock::time_point& time_point,
                                                     const char* fmt) {
    auto tt = std::chrono::system_clock::to_time_t(time_point);
    auto tm = localtime(&tt);
    std::stringstream ss;
    ss << std::put_time(tm, fmt);
    if (!ss.fail()) {
      return ss.str();
    } else {
      throw std::runtime_error("Failed to format localtime");
    }
  }

  inline static std::string to_localtime_string(const std::chrono::system_clock::time_point& time_point) {
    return to_localtime_string_fmt(time_point, "%Y-%m-%d %H:%M:%S");
  }

  inline static std::string to_localdate_string(const std::chrono::system_clock::time_point& time_point) {
    return to_localtime_string_fmt(time_point, "%Y-%m-%d");
  }
}

