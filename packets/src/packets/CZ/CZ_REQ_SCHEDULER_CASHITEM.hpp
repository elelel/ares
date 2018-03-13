template <typename PacketSet>
struct type<PacketSet, CZ_REQ_SCHEDULER_CASHITEM> {
  using packet_set = PacketSet;
  using packet_name = CZ_REQ_SCHEDULER_CASHITEM;

  inline void emplace() {
    PacketType = packet_sets::id_of<packet_set, packet_name>::value;
  }

  uint16_t PacketType;
};
