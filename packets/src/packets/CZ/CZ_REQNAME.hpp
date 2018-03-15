template <typename PacketSet>
struct type<PacketSet, CZ_REQNAME> {
  using packet_set = PacketSet;
  using packet_name = CZ_REQNAME;

  inline void emplace(const uint32_t aid) {
    PacketType = packet_sets::id_of<packet_set, packet_name>::value;
    AID_ = aid;
  }

  inline explicit type(const uint32_t aid) {
    emplace(aid);
  }

  uint32_t AID() const {
    return AID_;
  }
  
  uint16_t PacketType;
private:
  uint32_t AID_;
};
