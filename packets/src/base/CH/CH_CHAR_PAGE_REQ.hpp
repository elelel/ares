struct CH_CHAR_PAGE_REQ {
  inline void emplace() {
    PacketType = packet_id::CH_CHAR_PAGE_REQ;
  }

  explicit CH_CHAR_PAGE_REQ() {
    emplace();
  }

  uint16_t PacketType;
};
