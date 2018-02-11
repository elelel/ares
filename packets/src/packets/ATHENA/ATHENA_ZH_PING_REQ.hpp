template <typename PacketSet>
struct type<PacketSet, ATHENA_ZH_PING_REQ> {
  using packet_set = PacketSet;
  using packet_name = ATHENA_ZH_PING_REQ;
  
  void emplace() {
    PacketType = packet_sets::id_of<PacketSet, packet_name>::value;
  }

  explicit type() {
    emplace();
  }
  
  uint16_t PacketType;
};
