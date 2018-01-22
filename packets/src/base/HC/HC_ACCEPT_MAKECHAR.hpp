struct HC_ACCEPT_MAKECHAR {
  inline void emplace() {
    PacketType = packet_id::HC_ACCEPT_MAKECHAR;
  }

  explicit HC_ACCEPT_MAKECHAR() {
    emplace();
  }
  
  const CHARACTER_INFO* character_info() const {
    return character_info_;
  }

  uint16_t PacketType;
private:
  CHARACTER_INFO character_info_[];
};
