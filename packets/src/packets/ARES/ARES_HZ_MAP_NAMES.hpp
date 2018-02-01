template <typename PacketSet>
struct type<PacketSet, ARES_HZ_MAP_NAMES> {
  using packet_name = ARES_HZ_MAP_NAMES;
  
  inline void emplace(const std::set<std::string>& maps) {
    PacketType = PacketSet::template id_of<type<PacketSet, packet_name>>::value;
    PacketLength = sizeof(*this);
    for (const auto& map : maps) {
      PacketLength += map.size() + 1;
    }
  }

  inline void emplace(const size_t sz) {
    PacketType = PacketSet::template id_of<type<PacketSet, packet_name>>::value;
    PacketLength = sizeof(*this) + sz;
  }

  explicit type(const std::set<std::string>& maps) {
    emplace(maps);
  }

  explicit type(const size_t sz) {
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
