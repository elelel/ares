#pragma once

#include "../fixed_string.hpp"

template <size_t Size>
inline void ares::model::fixed_string<Size>::emplace(const std::string& s) {
  copy_buf_with_zero_pad(data_, Size, s.c_str(), s.size());  
}

template <size_t Size>
inline void ares::model::fixed_string<Size>::emplace(const void* buf, const size_t buf_sz) {
  copy_buf_with_zero_pad(data_, Size, buf, buf_sz);
}

template <size_t Size>
inline ares::model::fixed_string<Size>::fixed_string(const std::string& s) {
  emplace(s);
}

template <size_t Size>
inline ares::model::fixed_string<Size>::fixed_string(const void* buf, const size_t buf_sz) {
  emplace(buf, buf_sz);
}

template <size_t Size>
template <size_t OtherSize>
inline ares::model::fixed_string<Size>::fixed_string(const fixed_string<OtherSize>& other) {
  copy_buf_with_zero_pad(data_, Size, other.data_, OtherSize);
}

template <size_t Size>
template <size_t OtherSize>
inline auto ares::model::fixed_string<Size>::operator=(const fixed_string<OtherSize>& other) -> fixed_string<Size>& {
  copy_buf_with_zero_pad(data_, Size, other.data_, OtherSize);
  return *this;
}

template <size_t Size>
inline bool ares::model::fixed_string<Size>::operator==(const fixed_string<Size>& other) const {
  return strncmp(data_, other.data_, Size) == 0;
}

template <size_t Size>
inline ares::model::fixed_string<Size>::operator const char*() const {
  return data_;
}

template <size_t Size>
inline ares::model::fixed_string<Size>::operator const void*() const {
  return data_;
}

template <size_t Size>
inline const void* ares::model::fixed_string<Size>::data() const {
  return data_;
}
