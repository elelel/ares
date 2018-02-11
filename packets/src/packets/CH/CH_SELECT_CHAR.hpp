template <typename PacketSet>
struct type<PacketSet, CH_SELECT_CHAR> {
  using packet_set = PacketSet;
  using packet_name = CH_SELECT_CHAR;
  
  inline void emplace(const uint8_t CharNum) {
    PacketType = PacketSet::template id_of<CH_SELECT_CHAR>::value;
    CharNum_ = CharNum;
  }

  explicit type(const uint8_t CharNum) {
    emplace(CharNum);
  }

  uint8_t CharNum() const {
    return CharNum_;
  }

  uint16_t PacketType;
private:
  uint8_t CharNum_;
};
