template <typename PacketSet>
struct type<PacketSet, CZ_LESSEFFECT> {
  using packet_set = PacketSet;
  using packet_name = CZ_LESSEFFECT;

  inline void emplace(const uint32_t state) {
    PacketType = packet_sets::id_of<packet_set, packet_name>::value;
    state_ = state;
  }

  inline explicit type(const uint32_t state) {
    emplace(state);
  }

  inline uint32_t state() const {
    return state_;
  }

  uint16_t PacketType;
private:
  uint32_t state_;
};
