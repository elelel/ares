template <typename PacketSet>
struct type<PacketSet, ARES_HA_PING_REQ> {
  using packet_set = PacketSet;
  using packet_name = ARES_HA_PING_REQ;
  
  void emplace() {
    PacketType = packet_sets::id_of<PacketSet, packet_name>::value;
  }

  explicit type() {
    emplace();
  }
  
  uint16_t PacketType;
};
