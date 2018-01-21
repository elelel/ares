struct PING {
  inline void emplace(const uint32_t aid) {
    PacketType = packet_id::PING;
    aid_ = aid;
  }

  explicit PING(const uint32_t aid) {
    emplace(aid);
  }

  uint32_t aid() const {
    return aid_;
  }
  
  uint16_t PacketType;
private:
  uint32_t aid_;
};
