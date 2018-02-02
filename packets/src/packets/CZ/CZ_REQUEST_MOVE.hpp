template <typename PacketSet>
struct type<PacketSet, CZ_REQUEST_MOVE> {
  using packet_name = CZ_REQUEST_MOVE;
  
  inline void emplace(const uint16_t to_x, const uint16_t to_y) {
    PacketType = packet_sets::id_of<PacketSet, packet_name>::value;
    dest_[0] = to_x >> 2;
    dest_[1] = (to_x << 6) | ((to_y >> 4) & 0x3f);
    dest_[2] = to_y << 4;
  }

  inline void emplace(const uint16_t to_x, const uint16_t to_y, const uint8_t dir) {
    emplace(to_x, to_y);
    dest_[2] = dest_[2] | dir;
  }

  explicit type(const uint16_t to_x, const uint16_t to_y) {
    emplace(to_x, to_y);
  }

  explicit type(const uint16_t to_x, const uint16_t to_y, uint8_t dir) {
    emplace(to_x, to_y, dir);
  }

  inline uint16_t to_x() const {
    return (dest_[0] << 2) | (dest_[1] >> 6);
  }

  inline uint16_t to_y() const {
    return ((dest_[1] & 0x3f) << 4) | (dest_[2] >> 4);
  }

  inline uint8_t dir() const {
    return dest_[2] & 0xf;
  }
  
  uint16_t PacketType;
private:
  uint8_t dest_[3];
};
