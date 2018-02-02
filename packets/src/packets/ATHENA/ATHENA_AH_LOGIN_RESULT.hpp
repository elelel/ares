template <typename PacketSet>
struct type<PacketSet, ATHENA_AH_LOGIN_RESULT> {
  using packet_name = ATHENA_AH_LOGIN_RESULT;
  
  void emplace(const uint8_t ErrorCode) {
    PacketType = packet_sets::id_of<PacketSet, packet_name>::value;
    ErrorCode_ = ErrorCode;
  }
  
  explicit type(const uint8_t ErrorCode) {
    emplace(ErrorCode);
  }

  uint8_t ErrorCode() {
    return ErrorCode_;
  }
  
  uint16_t PacketType;
private:
  uint8_t ErrorCode_;
};
