template <typename PacketSet>
struct type<PacketSet, HC_CHAR_PAGES> {
  using packet_set = PacketSet;
  using packet_name = HC_CHAR_PAGES;
  
  inline void emplace(const size_t nchars) {
    PacketType = packet_sets::id_of<PacketSet, packet_name>::value;
    PacketLength = sizeof(*this) +  nchars * sizeof(CHARACTER_INFO);
  }

  explicit type(const size_t nchars) {
    emplace(nchars);
  }

  uint16_t PacketType;
  uint16_t PacketLength;

  const CHARACTER_INFO* characters() const {
    return characters_;
  }
  
private:
  CHARACTER_INFO characters_[];
};
