#pragma once

/*! \file utils.hpp
  \brief Misc utility functions
*/

#include <sstream>
#include <chrono>
#include <iomanip>

namespace ares {
  /*! Copy raw bytes to a destination buffer of a predefined size and set unused destination bytes to 0.
    This is to avoid pessimistic initialization (zeroing out the whole dest buffer before copying
    \param dest destination buffer to copy the bytes to
    \param dest_sz destination buffer size
    \param src source buffer to copy the bytes from
    \param src_sz number of meaningful bytes in source buffer
  */
  inline static void copy_buf_with_zero_pad(void* dest, const size_t dest_sz, const void* src, const size_t src_sz) {
    auto len = (src_sz > dest_sz) ? dest_sz : src_sz;
    memcpy(dest, src, len);
    memset((void*)((uintptr_t)dest + len), 0, dest_sz - len);
  }

  /*! Convert time point to host epoch time 
    \param time_point time point to convert
  */
  inline static uint32_t to_host_epoch_time(const std::chrono::system_clock::time_point& time_point) {
    return std::chrono::duration_cast<std::chrono::seconds>(time_point.time_since_epoch()).count();
  }

  /*! Format time point as GMT string using a custom format
    \param time_point time point to format
    \param fmt time format to use */
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

  /*! Format time point as GMT string using predefined date/time format
    \param time_point time point to format
  */
  inline static std::string to_gmtime_string(const std::chrono::system_clock::time_point& time_point) {
    return to_gmtime_string_fmt(time_point, "%Y-%m-%d %H:%M:%S");
  }

  /*! Format time point as GMT string using predefined date format
    \param time_point time point to format
  */
  inline static std::string to_gmdate_string(const std::chrono::system_clock::time_point& time_point) {
    return to_gmtime_string_fmt(time_point, "%Y-%m-%d");
  }

  /*! Format time point as local time string using a custom format
    \param time_point time point to format
    \param fmt time format to use */
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

  /*! Format time point as local time string using predefined date/time format
    \param time_point time point to format
  */
  inline static std::string to_localtime_string(const std::chrono::system_clock::time_point& time_point) {
    return to_localtime_string_fmt(time_point, "%Y-%m-%d %H:%M:%S");
  }

  /*! Format time point as local time string using a custom format
    \param time_point time point to format
    \param fmt time format to use */
  inline static std::string to_localdate_string(const std::chrono::system_clock::time_point& time_point) {
    return to_localtime_string_fmt(time_point, "%Y-%m-%d");
  }
}

