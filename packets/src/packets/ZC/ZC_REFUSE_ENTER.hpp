template <typename PacketSet>
struct type<PacketSet, ZC_REFUSE_ENTER> {
  using packet_set = PacketSet;
  using packet_name = ZC_REFUSE_ENTER;

  static const uint8_t client_type_mismatch = 0;
  static const uint8_t id_mismatch = 1;
  static const uint8_t out_of_time = 2;
  static const uint8_t already_logged_in = 3;
  static const uint8_t waiting_state = 4;

  inline void emplace(const uint8_t error_code) {
    PacketType = packet_sets::id_of<PacketSet, packet_name>::value;
    ErrorCode_ = error_code;
  }

  type(const uint8_t error_code) {
    emplace(error_code);
  }

  uint8_t ErrorCode() const {
    return ErrorCode_;
  }

  uint16_t PacketType;
private:
  uint8_t ErrorCode_;
};
