#pragma once

#include "pc.hpp"

namespace ares {
  namespace model {
    struct map_cell_gat_type;
    struct map_cell_flags;
    
    struct map_cell_gat_type {
      static map_cell_gat_type from_gat_uint32(const uint32_t& gat_uint32);
      explicit map_cell_gat_type(const map_cell_flags& flags);

      map_cell_gat_type(const map_cell_gat_type& other);
      map_cell_gat_type& operator=(const map_cell_gat_type& other);
      
      const uint32_t& data() const;
    private:
      uint32_t data_;

      map_cell_gat_type(const uint32_t& gat_uint32);
    };
    
    struct map_cell_flags {
      static constexpr uint8_t walkable = 0x01;
      static constexpr uint8_t shootable = 0x02;
      static constexpr uint8_t water = 0x04;
      
      static map_cell_flags from_uint8(const uint8_t& uint8);
      explicit map_cell_flags(const map_cell_gat_type& gat_type);

      map_cell_flags();
      map_cell_flags(const map_cell_flags& other);
      map_cell_flags& operator=(const map_cell_flags& other);

      const uint8_t& data() const;
    private:
      uint8_t data_;

      map_cell_flags(const uint8_t& flags);
    };

    struct map_info {
      map_info(const uint16_t& x_size, const uint16_t& y_size);
      
      dense_lattice<map_cell_flags>& static_flags();
      const dense_lattice<map_cell_flags>& static_flags() const;
      inline map_cell_flags& static_flags(const uint16_t& x, const uint16_t& y);
      inline const map_cell_flags& static_flags(const uint16_t& x, const uint16_t& y) const;
      
      inline void set_dynamic_flags(const uint16_t& x, const uint16_t& y, const map_cell_flags& flags);
      inline std::optional<map_cell_flags> dynamic_flags(const uint16_t& x, const uint16_t& y) const;

      inline uint16_t x_size() const;
      inline uint16_t y_size() const;
    private:
      dense_lattice<map_cell_flags> static_flags_;
      sparse_lattice<map_cell_flags> dynamic_flags_;
      uint16_t x_size_;
      uint16_t y_size_;

      size_t shift_;
    };
  }
}
