struct HC_REFUSE_ENTER {
  void emplace(const uint8_t ErrorCode) {
    PacketType = packet_id::HC_REFUSE_ENTER;
    ErrorCode_ = ErrorCode;
  }

  explicit HC_REFUSE_ENTER(const uint8_t ErrorCode) {
    emplace(ErrorCode);
  }
                      
  uint16_t PacketType;
private:
  uint8_t ErrorCode_;
};
