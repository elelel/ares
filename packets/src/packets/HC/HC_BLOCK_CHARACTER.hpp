template <typename PacketSet>
struct type<PacketSet, HC_BLOCK_CHARACTER> {
  using packet_set = PacketSet;
  using packet_name = HC_BLOCK_CHARACTER;
  
  inline void emplace() {
    PacketType = packet_sets::id_of<PacketSet, packet_name>::value;
    PacketLength = sizeof(*this);
  }

  inline void emplace(const size_t expire_time_sz) {
    PacketType = packet_sets::id_of<PacketSet, packet_name>::value;
    PacketLength = sizeof(*this) + expire_time_sz;
  }

  explicit type() {
    emplace();
  }

  explicit type(const size_t expire_time_sz) {
    emplace(expire_time_sz);
  }
  
  const char* expire_time() const {
    return expire_time_;
  }
  
  uint16_t PacketType;
  uint16_t PacketLength;
private:
  char expire_time_[];
};
