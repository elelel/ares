#pragma once

#include <string.h>

#include "../location.hpp"
#include "../utils.hpp"

inline ares::model::map_name_string::map_name_string() :
  fixed_string() {
}

inline ares::model::map_name_string::map_name_string(const map_name_ext_string& s) :
  fixed_string(s, uintptr_t(strchr(s, '.')) - uintptr_t(s.data())) {
}

inline auto ares::model::map_name_string::operator=(const map_name_ext_string& s) -> map_name_string& {
  copy_buf_with_zero_pad(data_, size::value, s, uintptr_t(strchr(s, '.')) - uintptr_t(s.data()));
  return *this;
}

inline auto ares::model::map_name_string::operator=(const std::string& s) -> map_name_string& {
  fixed_string::operator=(s);
  return *this;
}

inline bool ares::model::map_name_string::operator==(const map_name_ext_string& s) const {
  return strncmp(data_, s, size::value) == 0;
}

inline ares::model::map_name_ext_string::map_name_ext_string() :
  fixed_string() {
}
  
inline ares::model::map_name_ext_string::map_name_ext_string(const std::string& s) :
  fixed_string(s) {
  const char ext[] = ".gat";
  memcpy((void*)(uintptr_t(data_) + s.size()), ext, 4);
}

inline ares::model::map_name_ext_string::map_name_ext_string(const map_name_string& s) :
  fixed_string(s, map_name_string::size::value) {
  const char ext[] = ".gat";
  memcpy((void*)(uintptr_t(data_) + strlen(data_)), ext, 4);
}

inline auto ares::model::map_name_ext_string::operator=(const map_name_string& s) -> map_name_ext_string& {
  const auto other_len = strlen(data_);
  copy_buf_with_zero_pad(data_, size::value, s.data(), other_len);
  const char ext[] = ".gat";
  memcpy((void*)(uintptr_t(data_) + other_len), ext, 4);
  return *this;
}

inline auto ares::model::map_name_ext_string::operator=(const std::string& s) -> map_name_ext_string& {
  fixed_string::operator=(s + ".gat");
  return *this;
}

inline bool ares::model::map_name_ext_string::operator==(const map_name_string& s) const {
  return strncmp(data_, s, map_name_string::size::value) == 0;
}

inline ares::model::packed_coordinates::packed_coordinates() {
}

inline ares::model::packed_coordinates::packed_coordinates(const packed_coordinates& other) {
  data_[0] = other.data_[0];
  data_[1] = other.data_[1];
  data_[2] = other.data_[2];
}

inline void ares::model::packed_coordinates::emplace(const uint16_t x, const uint16_t y) {
  data_[0] = x >> 2;
  data_[1] = (x << 6) | ((y >> 4) & 0x3f);
  data_[2] = y << 4;
}

inline void ares::model::packed_coordinates::emplace(const uint16_t x, const uint16_t y, const uint8_t dir) {
  emplace(x, y);
  data_[2] = data_[2] | dir;
}

inline ares::model::packed_coordinates::packed_coordinates(const uint16_t x, const uint16_t y) {
  emplace(x, y);
}

inline ares::model::packed_coordinates::packed_coordinates(const uint16_t x, const uint16_t y, const uint8_t dir) {
  emplace(x, y, dir);
}

inline auto ares::model::packed_coordinates::operator=(const packed_coordinates& other) -> packed_coordinates& {
  data_[0] = other.data_[0];
  data_[1] = other.data_[1];
  data_[2] = other.data_[2];
  return *this;
}

inline bool ares::model::packed_coordinates::operator==(const packed_coordinates& other) const {
  return (data_[0] == other.data_[0]) && (data_[1] == other.data_[1]) && (data_[2] == other.data_[2]);
}

inline uint16_t ares::model::packed_coordinates::x() const {
  return (data_[0] << 2) | (data_[1] >> 6);
}

inline uint16_t ares::model::packed_coordinates::y() const {
  return ((data_[1] & 0x3f) << 4) | (data_[2] >> 4);
}

inline uint8_t ares::model::packed_coordinates::dir() const {
  return data_[2] & 0xf;
}

inline ares::model::coordinates::coordinates() {
}

inline ares::model::coordinates::coordinates(const uint16_t x, const uint16_t y) :
  data_(x | (y << sizeof(uint16_t))) {
}

inline ares::model::coordinates::coordinates(const uint32_t index) :
  data_(index) {
}

inline ares::model::coordinates::coordinates(const coordinates& other) :
  data_(other.data_) {
}

inline bool ares::model::coordinates::operator==(const coordinates& other) const {
  return data_ == other.data_;
}

inline bool ares::model::coordinates::operator!=(const coordinates& other) const {
  return !(*this == other);
}

inline auto ares::model::coordinates::operator=(const coordinates& other) -> coordinates& {
  data_ = other.data_;
  return *this;
}

inline bool ares::model::coordinates::operator<(const coordinates& other) const {
  return data_ < other.data_;
}

inline uint16_t* ares::model::coordinates::x() {
  return reinterpret_cast<uint16_t*>(&data_);
}

inline uint16_t* ares::model::coordinates::y() {
  return reinterpret_cast<uint16_t*>((uintptr_t)&data_ + sizeof(uint16_t));
}

inline uint32_t& ares::model::coordinates::index() {
  return data_;
}
