struct ATHENA_AH_PING_ACK {
  void emplace() {
    PacketType = packet_id::ATHENA_AH_PING_ACK;
  }

  explicit ATHENA_AH_PING_ACK() {
    emplace();
  }
  
  uint16_t PacketType;
};
