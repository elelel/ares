struct ATHENA_HA_ONLINE_AIDS {
  void emplace(const size_t users_sz) {
    PacketType = packet_id::ATHENA_HA_ONLINE_AIDS;
    PacketLength = sizeof(ATHENA_HA_ONLINE_AIDS) + users_sz * sizeof(uint32_t);
    users_sz_ = users_sz;
  }
  
  explicit ATHENA_HA_ONLINE_AIDS(const size_t users_sz) {
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
