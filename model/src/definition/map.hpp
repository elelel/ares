#pragma once

#include "../map.hpp"

inline ares::model::map_cell_gat_type::map_cell_gat_type(const uint32_t& gat_uint32) :
  data_(gat_uint32) {
}

inline auto ares::model::map_cell_gat_type::from_gat_uint32(const uint32_t& gat_uint32) -> map_cell_gat_type {
  return map_cell_gat_type(gat_uint32);
}

inline ares::model::map_cell_gat_type::map_cell_gat_type(const map_cell_flags& flags) {
  auto masked = flags.data() & (map_cell_flags::walkable | map_cell_flags::shootable | map_cell_flags::water);
  if (masked == (map_cell_flags::walkable | map_cell_flags::shootable)) data_ = 0;
  else if (masked == 0) data_ = 1;
  else if (masked == (map_cell_flags::walkable | map_cell_flags::shootable | map_cell_flags::water)) data_ = 3;
  else if (masked == (map_cell_flags::shootable)) data_ = 5;
  else throw std::runtime_error("can't create map cell gat type, unsupported map cell flags " + std::to_string(flags.data()));
}

inline ares::model::map_cell_gat_type::map_cell_gat_type(const map_cell_gat_type& other) :
  data_(other.data_) {
}

inline auto ares::model::map_cell_gat_type::operator=(const map_cell_gat_type& other) -> map_cell_gat_type& {
  data_ = other.data_;
  return *this;
}

inline const uint32_t& ares::model::map_cell_gat_type::data() const {
  return data_;
}

inline ares::model::map_cell_flags::map_cell_flags(const uint8_t& flags) :
  data_(flags) {
}

inline auto ares::model::map_cell_flags::from_uint8(const uint8_t& uint8) -> map_cell_flags {
  return map_cell_flags(uint8);
}

inline ares::model::map_cell_flags::map_cell_flags(const map_cell_gat_type& gat_type) {
  switch (gat_type.data()) {
  case 0: data_ = walkable | shootable; return;  // ground
  case 1: data_ = 0; return; // ground
  case 2: data_ = walkable | shootable; return;
  case 3: data_ = walkable | shootable | water; return; // water
  case 4: data_ = walkable | shootable; return;
  case 5: data_ = shootable; return;
  case 6: data_ = walkable | shootable; return;
  default: throw std::runtime_error("unknown map cell gat type " + std::to_string(gat_type.data()));
  }
}

inline ares::model::map_cell_flags::map_cell_flags(const map_cell_flags& other) :
  data_(other.data_) {
}

inline auto ares::model::map_cell_flags::operator=(const map_cell_flags& other) -> map_cell_flags& {
  data_ = other.data_;
  return *this;
}

inline const uint8_t& ares::model::map_cell_flags::data() const {
  return data_;
}
