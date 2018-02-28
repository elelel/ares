template <typename PacketSet>
struct type<PacketSet, ARES_HZ_MAP_IDS> {
  using packet_set = PacketSet;
  using packet_name = ARES_HZ_MAP_IDS;
  
  inline void emplace(const std::set<std::string>& maps) {
    PacketType = packet_sets::id_of<PacketSet, packet_name>::value;
    PacketLength = sizeof(*this);
    for (const auto& map : maps) {
      PacketLength += map.size() + 1;
    }
  }

  inline void emplace(const size_t sz) {
    PacketType = packet_sets::id_of<PacketSet, packet_name>::value;
    PacketLength = sizeof(*this) + sz;
  }

  explicit type(const std::set<std::string>& maps) {
    emplace(maps);
  }

  explicit type(const size_t sz) {
    emplace(sz);
  }

  const uint32_t* map_ids() const {
    return map_ids_;
  }
  
  uint16_t PacketType;
  uint16_t PacketLength;
private:
  uint32_t map_ids_[];
};
