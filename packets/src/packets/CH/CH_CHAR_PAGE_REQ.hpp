template <typename PacketSet>
struct type<PacketSet, CH_CHAR_PAGE_REQ> {
  using packet_set = PacketSet;
  using packet_name = CH_CHAR_PAGE_REQ;
  
  inline void emplace() {
    PacketType = PacketSet::template id_of<CH_CHAR_PAGE_REQ>::value;
  }

  explicit type() {
    emplace();
  }

  uint16_t PacketType;
};
