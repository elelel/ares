template <typename PacketSet>
struct type<PacketSet, ARES_AH_LOGIN_RESULT> {
  using packet_set = PacketSet;
  using packet_name = ARES_AH_LOGIN_RESULT;
  
  inline void emplace(const uint8_t error_code) {
    PacketType = packet_sets::id_of<packet_set, packet_name>::value;
    error_code_ = error_code;
  }
  
  inline explicit type(const uint8_t error_code) {
    emplace(error_code);
  }

  inline uint8_t error_code() const {
    return error_code_;
  }
  
  uint16_t PacketType;
private:
  uint8_t error_code_;
};
