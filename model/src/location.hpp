#pragma once

namespace ares {
  namespace model {
    struct map_name_string;
    struct map_name_ext_string;
      
    struct map_name_string : fixed_string<12> {
      using fixed_string<12>::fixed_string;

      map_name_string();
      map_name_string(const map_name_ext_string& s);
      map_name_string& operator=(const map_name_ext_string& s);
      map_name_string& operator=(const std::string& s);
      bool operator==(const map_name_ext_string& s) const;
    };
      
    struct map_name_ext_string : fixed_string<16> {
      using fixed_string<16>::fixed_string;

      map_name_ext_string();
      map_name_ext_string(const std::string& s);
      map_name_ext_string(const map_name_string& s);
      map_name_ext_string& operator=(const map_name_string& s);
      map_name_ext_string& operator=(const std::string& s);
      bool operator==(const map_name_string& s) const;
    };

    struct packed_coordinates {
      packed_coordinates();

      static const uint8_t DIR_CENTER = -1;
      static const uint8_t DIR_NORTH = 0;
      static const uint8_t DIR_NORTHWEST = 1;
      static const uint8_t DIR_WEST = 2;
      static const uint8_t DIR_SOUTHWEST = 3;
      static const uint8_t DIR_SOUTH = 4;
      static const uint8_t DIR_SOUTHEAST = 5;
      static const uint8_t DIR_EAST = 6;
      static const uint8_t DIR_NORTHEAST = 7;
      
      inline void emplace(const uint16_t x, const uint16_t y);
      inline void emplace(const uint16_t x, const uint16_t y, const uint8_t dir);
      inline packed_coordinates(const uint16_t x, const uint16_t y);
      inline packed_coordinates(const uint16_t x, const uint16_t y, const uint8_t dir);
      inline packed_coordinates(const packed_coordinates& other);
      inline packed_coordinates& operator=(const packed_coordinates& other);
      inline bool operator==(const packed_coordinates& other) const;
      inline bool operator<(const packed_coordinates& other) const;

      inline uint16_t x() const;
      inline uint16_t y() const;
      inline uint8_t dir() const;
    private:
      uint8_t data_[3];
    };

    struct packed_coordinates2 {
      inline packed_coordinates2();

      inline void emplace(const uint16_t x0, const uint16_t y0, const uint16_t x1, const uint16_t y1, const uint8_t sx0, const uint8_t sy0);
      inline packed_coordinates2(const uint16_t x0, const uint16_t y0, const uint16_t x1, const uint16_t y1, const uint8_t sx0, const uint8_t sy0);
      inline uint16_t x0() const;
      inline uint16_t y0() const;
      inline uint16_t x1() const;
      inline uint16_t y1() const;
      inline uint8_t sx0() const;
      inline uint8_t sy0() const;
    private:
      uint8_t data_[6];
    };

    struct coordinates {
      coordinates();
      coordinates(const uint16_t x, const uint16_t y);
      coordinates(const uint32_t index);
      coordinates(const coordinates& other);
      coordinates& operator=(const coordinates& other);
      bool operator==(const coordinates& other) const;
      bool operator!=(const coordinates& other) const;
      bool operator<(const coordinates& other) const;
      uint16_t* x();
      uint16_t* y();
      uint32_t& index();
    private:
      uint32_t data_;
    };

    struct map_location {
      uint32_t map_id;
      coordinates coords;
    };
  }
}
