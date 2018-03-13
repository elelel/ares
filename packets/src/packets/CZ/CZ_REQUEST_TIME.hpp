template <typename PacketSet>
struct type<PacketSet, CZ_REQUEST_TIME> {
  using packet_set = PacketSet;
  using packet_name = CZ_REQUEST_TIME;

  inline void emplace() {
    PacketType = packet_sets::id_of<packet_set, packet_name>::value;
  }

  inline explicit type() {
    emplace();
  }

  uint32_t clientTime() const {
    return clientTime_;
  }

  uint16_t PacketType;
private:
  uint32_t clientTime_;
};
