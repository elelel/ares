template <typename PacketSet>
struct type<PacketSet, ARES_HA_USER_COUNT> {
  using packet_set = PacketSet;
  using packet_name = ARES_HA_USER_COUNT;
  
  inline void emplace(const uint32_t count) {
    PacketType = packet_sets::id_of<packet_set, packet_name>::value;
    count_ = count;
  }
  
  inline explicit type(const uint32_t count) {
    emplace(count);
  }
  
  inline uint32_t count() const {
    return count_;
  }
  
  uint16_t PacketType;
private:
  uint32_t count_;
};
