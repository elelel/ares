template <typename PacketSet>
struct type<PacketSet, ARES_ZH_MAP_IDS_REQ> {
  using packet_set = PacketSet;
  using packet_name = ARES_ZH_MAP_IDS_REQ;
  
  inline void emplace(const size_t buf_sz) {
    PacketType = packet_sets::id_of<PacketSet, packet_name>::value;
    buf_sz_ = buf_sz;
  }

  explicit type(const size_t buf_sz) {
    emplace(buf_sz);
  }

  uint32_t buf_sz() const {
    return buf_sz_;
  }

  uint16_t PacketType;
private:
  uint32_t buf_sz_;
};
