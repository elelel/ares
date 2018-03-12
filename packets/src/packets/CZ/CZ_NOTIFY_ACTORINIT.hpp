template <typename PacketSet>
struct type<PacketSet, CZ_NOTIFY_ACTORINIT> {
  using packet_set = PacketSet;
  using packet_name = CZ_NOTIFY_ACTORINIT;

  inline void emplace() {
    PacketType = packet_sets::id_of<packet_set, packet_name>::value;
  }

  inline explicit type() {
    emplace();
  }

  uint16_t PacketType;
};
