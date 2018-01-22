struct HC_CHAR_PAGES {
  inline void emplace(const size_t nchars) {
    PacketType = packet_id::HC_CHAR_PAGES;
    PacketLength = sizeof(*this) +  nchars * sizeof(CHARACTER_INFO);
  }

  explicit HC_CHAR_PAGES(const size_t nchars) {
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
