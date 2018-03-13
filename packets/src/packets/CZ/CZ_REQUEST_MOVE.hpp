template <typename PacketSet>
struct type<PacketSet, CZ_REQUEST_MOVE> {
  using packet_set = PacketSet;
  using packet_name = CZ_REQUEST_MOVE;
  
  inline void emplace(const model::packed_coordinates& coords) {
    PacketType = packet_sets::id_of<PacketSet, packet_name>::value;
    coords_ = coords;
  }

  template <typename... Args>
  inline void emplace(Args&&... args) {
    PacketType = packet_sets::id_of<PacketSet, packet_name>::value;
    coords_ = std::move(model::packed_coordinates(std::forward<Args>(args)...));
  }

  inline explicit type(const model::packed_coordinates& coords) {
    emplace(coords);
  }
  
  template <typename... Args>
  inline type(Args&&... args) {
    emplace(std::forward<Args>(args)...);
  }

  inline const model::packed_coordinates& coords() const {
    return coords_;
  }
  
  uint16_t PacketType;
private:
  model::packed_coordinates coords_;
};
