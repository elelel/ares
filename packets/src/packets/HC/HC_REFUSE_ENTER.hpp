template <typename PacketSet>
struct type<PacketSet, HC_REFUSE_ENTER> {
  using packet_set = PacketSet;
  using packet_name = HC_REFUSE_ENTER;
  
  void emplace(const uint8_t ErrorCode) {
    PacketType = packet_sets::id_of<PacketSet, packet_name>::value;
    ErrorCode_ = ErrorCode;
  }

  explicit type(const uint8_t ErrorCode) {
    emplace(ErrorCode);
  }
                      
  uint16_t PacketType;
private:
  uint8_t ErrorCode_;
};
