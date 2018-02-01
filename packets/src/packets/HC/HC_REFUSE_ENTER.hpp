template <typename PacketSet>
struct type<PacketSet, HC_REFUSE_ENTER> {
  using packet_name = HC_REFUSE_ENTER;
  
  void emplace(const uint8_t ErrorCode) {
    PacketType = PacketSet::template id_of<type<PacketSet, packet_name>>::value;
    ErrorCode_ = ErrorCode;
  }

  explicit type(const uint8_t ErrorCode) {
    emplace(ErrorCode);
  }
                      
  uint16_t PacketType;
private:
  uint8_t ErrorCode_;
};
