struct ARES_ZH_MAP_NAMES_REQ {
  inline void emplace(const size_t buf_sz) {
    PacketType = packet_id::ARES_ZH_MAP_NAMES_REQ;
    buf_sz_ = buf_sz;
  }

  explicit ARES_ZH_MAP_NAMES_REQ(const size_t buf_sz) {
    emplace(buf_sz);
  }

  uint32_t buf_sz() const {
    return buf_sz_;
  }

  uint16_t PacketType;
private:
  uint32_t buf_sz_;
};
