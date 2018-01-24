struct CH_SELECT_CHAR {
  inline void emplace(const uint8_t CharNum) {
    PacketType = packet_id::CH_SELECT_CHAR;
    CharNum_ = CharNum;
  }

  explicit CH_SELECT_CHAR(const uint8_t CharNum) {
    emplace(CharNum);
  }

  uint8_t CharNum() const {
    return CharNum_;
  }

  uint16_t PacketType;
private:
  uint8_t CharNum_;
};
