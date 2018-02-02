template <typename PacketSet>
struct type<PacketSet, ATHENA_HA_ONLINE_AIDS> {
  using packet_name = ATHENA_HA_ONLINE_AIDS;
  
  void emplace(const size_t users_sz) {
    PacketType = packet_sets::id_of<PacketSet, packet_name>::value;
    PacketLength = sizeof(*this) + users_sz * sizeof(uint32_t);
    users_sz_ = users_sz;
  }
  
  explicit type(const size_t users_sz) {
    emplace(users_sz);
  }
  
  uint32_t users_sz() {
    return users_sz_;
  }

  const uint32_t* users() {
    return users_;
  }
  
  uint16_t PacketType;
  uint16_t PacketLength;
private:
  uint32_t users_sz_;
  uint32_t users_[];
};
