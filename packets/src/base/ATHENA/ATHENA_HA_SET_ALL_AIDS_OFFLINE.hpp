struct ATHENA_HA_SET_ALL_AIDS_OFFLINE {
  void emplace() {
    PacketType = packet_id::ATHENA_HA_SET_ALL_AIDS_OFFLINE;
  }
  
  explicit ATHENA_HA_SET_ALL_AIDS_OFFLINE() {
    emplace();
  }
  
  uint16_t PacketType;
};
