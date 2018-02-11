template <typename PacketSet>
struct type<PacketSet, HC_ACCEPT_MAKECHAR> {
  using packet_set = PacketSet;
  using packet_name = HC_ACCEPT_MAKECHAR;
  
  inline void emplace() {
    PacketType = packet_sets::id_of<PacketSet, packet_name>::value;
  }

  explicit type() {
    emplace();
  }
  
  const CHARACTER_INFO* character_info() const {
    return character_info_;
  }

  uint16_t PacketType;
private:
  CHARACTER_INFO character_info_[];
};
