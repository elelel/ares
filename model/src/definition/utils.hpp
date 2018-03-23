#pragma once

#include "../utils.hpp"

inline void ares::model::copy_buf_with_zero_pad(void* dest, const size_t dest_sz, const void* src, const size_t src_sz) {
  const auto len = (src_sz > dest_sz) ? dest_sz : src_sz;
  memcpy(dest, src, len);
  memset((void*)((uintptr_t)dest + len), 0, dest_sz - len);
}

inline uint32_t ares::model::to_host_epoch_time(const std::chrono::system_clock::time_point& time_point) {
  return uint32_t(std::chrono::duration_cast<std::chrono::seconds>(time_point.time_since_epoch()).count());
}

inline std::string ares::model::to_gmtime_string_fmt(const std::chrono::system_clock::time_point& time_point,
                                                            const char* fmt) {
  auto tt = std::chrono::system_clock::to_time_t(time_point);
  auto tm = gmtime(tt);
  std::stringstream ss;
  ss << std::put_time(&tm, fmt);
  if (!ss.fail()) {
    return ss.str();
  } else {
    throw std::runtime_error("Failed to format gmtime");
  }
}

inline std::string ares::model::to_gmtime_string(const std::chrono::system_clock::time_point& time_point) {
  return to_gmtime_string_fmt(time_point, "%Y-%m-%d %H:%M:%S");
}

inline std::string ares::model::to_gmdate_string(const std::chrono::system_clock::time_point& time_point) {
  return to_gmtime_string_fmt(time_point, "%Y-%m-%d");
}

inline std::string ares::model::to_localtime_string_fmt(const std::chrono::system_clock::time_point& time_point,
                                                               const char* fmt) {
  auto tt = std::chrono::system_clock::to_time_t(time_point);
  auto tm = localtime(tt);
  std::stringstream ss;
  ss << std::put_time(&tm, fmt);
  if (!ss.fail()) {
    return ss.str();
  } else {
    throw std::runtime_error("Failed to format localtime");
  }
}

inline std::string ares::model::to_localtime_string(const std::chrono::system_clock::time_point& time_point) {
  return to_localtime_string_fmt(time_point, "%Y-%m-%d %H:%M:%S");
}

inline std::string ares::model::to_localdate_string(const std::chrono::system_clock::time_point& time_point) {
  return to_localtime_string_fmt(time_point, "%Y-%m-%d");
}

inline std::tm ares::model::gmtime(const std::time_t& time_tt) {
  std::tm tm;
#ifdef _WIN32
  gmtime_s(&tm, &time_tt);
#else
  gmtime_r(&time_tt, &tm);
#endif
  return tm;
}

inline std::tm ares::model::localtime(const std::time_t& time_tt) {
  std::tm tm;
#ifdef _WIN32
  localtime_s(&tm, &time_tt);
#else
  localtime_r(&time_tt, &tm);
#endif
  return tm;
}

