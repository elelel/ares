struct ATHENA_AH_KICK_AID {
  inline void emplace(const uint32_t aid) {
    PacketType = packet_id::ATHENA_AH_KICK_AID;
    aid_ = aid;
  }
  
  explicit ATHENA_AH_KICK_AID(const uint32_t aid) {
    emplace(aid);
  }

  inline uint32_t aid() const {
    return aid_;
  }
  
  uint16_t PacketType;
private:
  uint32_t aid_;
};
