struct ATHENA_HA_SET_AID_ONLINE {
  void emplace(const uint32_t aid) {
    PacketType = packet_id::ATHENA_HA_SET_AID_ONLINE;
    aid_ = aid;
  }
  
  explicit ATHENA_HA_SET_AID_ONLINE(const uint32_t aid) {
    emplace(aid);
  }

  uint32_t aid() const {
    return aid_;
  }
  
  uint16_t PacketType;
private:
  uint32_t aid_;
};
