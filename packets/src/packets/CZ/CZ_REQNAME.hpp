template <typename PacketSet>
struct type<PacketSet, CZ_REQNAME> {
  using packet_set = PacketSet;
  using packet_name = CZ_REQNAME;

  inline void emplace(const uint32_t id) {
    PacketType = packet_sets::id_of<packet_set, packet_name>::value;
    id_ = id;
  }

  inline explicit type(const uint32_t id) {
    emplace(id);
  }

  uint32_t id() const {
    return id_;
  }
  
  uint16_t PacketType;
private:
  uint32_t id_;
};
