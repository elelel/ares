template <typename PacketSet>
struct type<PacketSet, ARES_AH_PING_ACK> {
  using packet_set = PacketSet;
  using packet_name = ARES_AH_PING_ACK;
  
  inline void emplace() {
    PacketType = packet_sets::id_of<packet_set, packet_name>::value;
  }

  inline explicit type() {
    emplace();
  }
  
  uint16_t PacketType;
};
