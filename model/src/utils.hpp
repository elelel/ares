#pragma once

/*! \file utils.hpp
  \brief Misc utility functions
*/

#include <chrono>
#include <cstring>
#include <iomanip>
#include <sstream>

namespace ares {
  namespace model {
    /*! Copy raw bytes to a destination buffer of a predefined size and set unused destination bytes to 0.
      This is to avoid pessimistic initialization (zeroing out the whole dest buffer before copying
      \param dest destination buffer to copy the bytes to
      \param dest_sz destination buffer size
      \param src source buffer to copy the bytes from
      \param src_sz number of meaningful bytes in source buffer
    */
    inline void copy_buf_with_zero_pad(void* dest, const size_t dest_sz, const void* src, const size_t src_sz);

    /*! Convert time point to host epoch time 
      \param time_point time point to convert
    */
    inline uint32_t to_host_epoch_time(const std::chrono::system_clock::time_point& time_point);

    /*! Format time point as GMT string using a custom format
      \param time_point time point to format
      \param fmt time format to use */
    inline std::string to_gmtime_string_fmt(const std::chrono::system_clock::time_point& time_point, const char* fmt);

    /*! Format time point as GMT string using predefined date/time format
      \param time_point time point to format
    */
    inline std::string to_gmtime_string(const std::chrono::system_clock::time_point& time_point);
    
    /*! Format time point as GMT string using predefined date format
      \param time_point time point to format
    */
    inline std::string to_gmdate_string(const std::chrono::system_clock::time_point& time_point);

    /*! Format time point as local time string using a custom format
      \param time_point time point to format
      \param fmt time format to use */
    inline std::string to_localtime_string_fmt(const std::chrono::system_clock::time_point& time_point, const char* fmt);

    /*! Format time point as local time string using predefined date/time format
      \param time_point time point to format
    */
    inline std::string to_localtime_string(const std::chrono::system_clock::time_point& time_point);
    
    /*! Format time point as local time string using a custom format
      \param time_point time point to format
      \param fmt time format to use */
    inline std::string to_localdate_string(const std::chrono::system_clock::time_point& time_point);

    /*! OS-independent wrapper for gmtime_s
     */
    inline std::tm gmtime(const std::time_t& time_tt);

    /*! OS-independent wrapper for localtime_s
     */
    inline std::tm localtime(const std::time_t& time_tt);

  }
}
