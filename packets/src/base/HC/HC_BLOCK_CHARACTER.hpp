struct HC_BLOCK_CHARACTER {
  inline void emplace() {
    PacketType = packet_id::HC_BLOCK_CHARACTER;
    PacketLength = sizeof(*this);
  }

  inline void emplace(const size_t expire_time_sz) {
    PacketType = packet_id::HC_BLOCK_CHARACTER;
    PacketLength = sizeof(*this) + expire_time_sz;
  }

  explicit HC_BLOCK_CHARACTER() {
    emplace();
  }

  explicit HC_BLOCK_CHARACTER(const size_t expire_time_sz) {
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
