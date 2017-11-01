struct ATHENA_ZH_PING_REQ {
  void emplace() {
    PacketType = packet_id::ATHENA_ZH_PING_REQ;
  };

  explicit ATHENA_ZH_PING_REQ() {
    emplace();
  }
  
  uint16_t PacketType;
};
