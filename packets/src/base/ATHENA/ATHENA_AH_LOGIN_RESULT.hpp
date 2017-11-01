struct ATHENA_AH_LOGIN_RESULT {
  void emplace(const uint8_t ErrorCode) {
    PacketType = packet_id::ATHENA_AH_LOGIN_RESULT;
    ErrorCode_ = ErrorCode;
  }
  
  explicit ATHENA_AH_LOGIN_RESULT(const uint8_t ErrorCode) {
    emplace(ErrorCode);
  }

  uint8_t ErrorCode() {
    return ErrorCode_;
  }
  
  uint16_t PacketType;
private:
  uint8_t ErrorCode_;
};
