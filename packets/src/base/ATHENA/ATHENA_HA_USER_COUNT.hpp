struct ATHENA_HA_USER_COUNT {
  void emplace(const uint32_t count) {
    PacketType = packet_id::ATHENA_HA_USER_COUNT;
    count_ = count;
  }
  
  explicit ATHENA_HA_USER_COUNT(const uint32_t count) {
    emplace(count);
  }
  
  uint32_t count() const {
    return count_;
  }
  
  uint16_t PacketType;
private:
  uint32_t count_;
};
