template <typename PacketSet>
struct type<PacketSet, AC_REFUSE_LOGIN> {
  using packet_set = PacketSet;
  using packet_name = AC_REFUSE_LOGIN;
  
  inline void emplace(const uint8_t ErrorCode, const std::string& blockDate) {
    PacketType = packet_sets::id_of<packet_set, packet_name>::value;
    ErrorCode_ = ErrorCode;
    blockDate_ = blockDate;
  }

  inline void emplace(const uint8_t ErrorCode) {
    PacketType = packet_sets::id_of<packet_set, packet_name>::value;
    ErrorCode_ = ErrorCode;
    blockDate_ = std::string();
  }
  
  explicit inline type(const uint8_t ErrorCode, const std::string& blockDate) {
    emplace(ErrorCode, blockDate);
  }
  
  explicit inline type(const uint8_t ErrorCode) {
    emplace(ErrorCode);
  }

  const model::fixed_string<20>& blockDate() const {
    return blockDate_;
  }
                      
  uint16_t PacketType;
private:
  uint8_t ErrorCode_;
  model::fixed_string<20> blockDate_;
};
