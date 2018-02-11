template <typename PacketSet>
struct type<PacketSet, SC_NOTIFY_BAN> {
  using packet_set = PacketSet;
  using packet_name = SC_NOTIFY_BAN;
  
  struct code {
    static const uint8_t server_closed = 1;
    static const uint8_t already_logged_in = 2;
    static const uint8_t timeout = 3;
    static const uint8_t server_full = 4;
    static const uint8_t underaged = 5;
    static const uint8_t last_connection_remains = 8;
    static const uint8_t too_many_connections_from_ip = 9;
    static const uint8_t payment_expired = 10;
    static const uint8_t kicked = 11;
  };
  
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
