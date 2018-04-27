template <typename PacketSet>
struct type<PacketSet, ZC_NOTIFY_PLAYERMOVE> {
  using packet_set = PacketSet;
  using packet_name = ZC_NOTIFY_PLAYERMOVE;
  
  inline void emplace(const uint16_t x, const uint16_t y, const uint16_t to_x, const uint16_t to_y) {
    PacketType = packet_sets::id_of<packet_set, packet_name>::value;
    auto duration = std::chrono::steady_clock::now().time_since_epoch();
    moveStartTime_ = std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();
    MoveData_.emplace(x, y, to_x, to_y, 8, 8);
  }

  inline explicit type(const uint16_t x, const uint16_t y, const uint16_t to_x, const uint16_t to_y) {
    emplace(x, y, to_x, to_y);
  }

  uint16_t PacketType;
private:
  uint32_t moveStartTime_;
  model::packed_coordinates2 MoveData_;
};
