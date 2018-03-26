#pragma once

#include <string>

/*!
  \file fixed_string.hpp
  Wrapper for fixed-size C-style strings. Should be converted to string slices later
 */

namespace ares {
  namespace model {
    template <size_t Size>
    struct fixed_string {
      using size = std::integral_constant<size_t, Size>;

      fixed_string() {};
      
      void emplace(const std::string& s);
      void emplace(const void* buf, const size_t buf_sz);
      
      fixed_string(const std::string& s);
      fixed_string(const void* buf, const size_t buf_sz);
      template <size_t OtherSize>
      fixed_string(const fixed_string<OtherSize>& other);
      template <size_t OtherSize>
      fixed_string<Size>& operator=(const fixed_string<OtherSize>& other);
      bool operator==(const fixed_string<Size>& other) const;

      operator const char* () const;
      operator const void* () const;

      const void* data() const;
    protected:
      char data_[Size];
    };
  }
}
