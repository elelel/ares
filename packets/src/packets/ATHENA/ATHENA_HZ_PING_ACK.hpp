template <typename PacketSet>
struct type<PacketSet, ATHENA_HZ_PING_ACK> {
  using packet_name = ATHENA_HZ_PING_ACK;
  
  void emplace() {
    PacketType = PacketSet::template id_of<type<PacketSet, packet_name>>::value;
  }

  explicit type() {
    emplace();
  }
  
  uint16_t PacketType;
};
