struct ATHENA_HA_PING_REQ {
  void emplace() {
    PacketType = packet_id::ATHENA_HA_PING_REQ;
  }

  explicit ATHENA_HA_PING_REQ() {
    emplace();
  }
  
  uint16_t PacketType;
};
