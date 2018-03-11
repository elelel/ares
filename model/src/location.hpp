#pragma once

namespace ares {
  namespace model {
    struct map_name_string;
    struct map_name_ext_string;
      
    struct map_name_string : fixed_string<12> {
      using fixed_string<12>::fixed_string;

      map_name_string(const map_name_ext_string& s);
      map_name_string& operator=(const map_name_ext_string& s);
      map_name_string& operator=(const std::string& s);
      bool operator==(const map_name_ext_string& s) const;
    };
      
    struct map_name_ext_string : fixed_string<16> {
      using fixed_string<16>::fixed_string;

      map_name_ext_string(const std::string& s);
      map_name_ext_string(const map_name_string& s);
      map_name_ext_string& operator=(const map_name_string& s);
      map_name_ext_string& operator=(const std::string& s);
      bool operator==(const map_name_string& s) const;
    };

    struct packed_coordinates {
      packed_coordinates();
      
      void emplace(const uint16_t x, const uint16_t y);
      void emplace(const uint16_t x, const uint16_t y, const uint8_t dir);
      packed_coordinates(const uint16_t x, const uint16_t y);
      packed_coordinates(const uint16_t x, const uint16_t y, const uint8_t dir);
      packed_coordinates(const packed_coordinates& other);
      packed_coordinates& operator=(const packed_coordinates& other);
      bool operator==(const packed_coordinates& other) const;

      uint16_t x() const;
      uint16_t y() const;
      uint8_t dir() const;
    private:
      uint8_t data_[3];
    };

    struct coordinates {
      uint16_t x;
      uint16_t y;
    };

    struct map_location {
      uint32_t map_id;
      coordinates coords;
    };
  }
}
