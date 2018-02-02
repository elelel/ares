template <typename PacketSet>
struct type<PacketSet, ATHENA_HA_USER_COUNT> {
  using packet_name = ATHENA_HA_USER_COUNT;
  
  void emplace(const uint32_t count) {
    PacketType = packet_sets::id_of<PacketSet, packet_name>::value;
    count_ = count;
  }
  
  explicit type(const uint32_t count) {
    emplace(count);
  }
  
  uint32_t count() const {
    return count_;
  }
  
  uint16_t PacketType;
private:
  uint32_t count_;
};
