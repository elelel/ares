template <typename PacketSet>
struct type<PacketSet, ZC_ACCEPT_ENTER> {
  using packet_set = PacketSet;
  using packet_name = ZC_ACCEPT_ENTER;

  inline void emplace(const std::chrono::system_clock::time_point& start_time,
                      const uint16_t x, const uint16_t y, const uint8_t dir,
                      const uint8_t x_size, const uint8_t y_size, const uint16_t font) {
    PacketType = packet_sets::id_of<packet_set, packet_name>::value;
    startTime_ = model::to_host_epoch_time(start_time);
    PosDir_ = model::packed_coordinates(x, y, dir);
    x_size_ = x_size;
    y_size_ = y_size;
    font_ = font;
  }

  explicit type(const std::chrono::system_clock::time_point& start_time,
                const uint16_t x, const uint16_t y, const uint8_t dir,
                const uint8_t x_size, const uint8_t y_size, const uint16_t font) {
    emplace(start_time, x, y, dir, x_size, y_size, font);
  }

  uint32_t startTime() const {
    return startTime_;
  }

  model::packed_coordinates PosDir() const {
    return PosDir_;
  }

  uint8_t x_size() const {
    return x_size_;
  }

  uint8_t y_size() const {
    return y_size_;
  }

  uint16_t font() const {
    return font_;
  }

  uint16_t PacketType;
  
private:
  uint32_t startTime_;
  model::packed_coordinates PosDir_;
  uint8_t x_size_;
  uint8_t y_size_;
  uint16_t font_;
};
