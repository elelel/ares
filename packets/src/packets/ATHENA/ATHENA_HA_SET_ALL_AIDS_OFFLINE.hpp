template <typename PacketSet>
struct type<PacketSet, ATHENA_HA_SET_ALL_AIDS_OFFLINE> {
  using packet_set = PacketSet;
  using packet_name = ATHENA_HA_SET_ALL_AIDS_OFFLINE;
  
  void emplace() {
    PacketType = packet_sets::id_of<PacketSet, packet_name>::value;
  }
  
  explicit type() {
    emplace();
  }
  
  uint16_t PacketType;
};
