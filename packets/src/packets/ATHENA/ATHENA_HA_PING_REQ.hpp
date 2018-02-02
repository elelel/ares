template <typename PacketSet>
struct type<PacketSet, ATHENA_HA_PING_REQ> {
  using packet_name = ATHENA_HA_PING_REQ;
  
  void emplace() {
    PacketType = packet_sets::id_of<PacketSet, packet_name>::value;
  }

  explicit type() {
    emplace();
  }
  
  uint16_t PacketType;
};
