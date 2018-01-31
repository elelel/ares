struct ARES_HZ_MAP_NAMES {
  inline void emplace(const std::set<std::string>& maps) {
    PacketType = packet_id::ARES_HZ_MAP_NAMES;
    PacketLength = sizeof(*this);
    for (const auto& map : maps) {
      PacketLength += map.size() + 1;
    }
  }

  inline void emplace(const size_t sz) {
    PacketType = packet_id::ARES_HZ_MAP_NAMES;
    PacketLength = sizeof(*this) + sz;
  }

  explicit ARES_HZ_MAP_NAMES(const std::set<std::string>& maps) {
    emplace(maps);
  }

  explicit ARES_HZ_MAP_NAMES(const size_t sz) {
    emplace(sz);
  }

  const char* map_names() const {
    return map_names_;
  }
  
  uint16_t PacketType;
  uint16_t PacketLength;
private:
  char map_names_[];
};
