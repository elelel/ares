template <typename PacketSet>
struct type<PacketSet, ARES_ZH_MAP_ID_LOADED> {
  using packet_set = PacketSet;
  using packet_name = ARES_ZH_MAP_ID_LOADED;

  inline void emplace(const uint32_t map_id) {
    PacketType = packet_sets::id_of<PacketSet, packet_name>::value;
    map_id_ = map_id;
  }

  inline explicit type(const uint32_t map_id) {
    emplace(map_id);
  }
  
  inline int32_t map_id() const {
    return map_id_;
  }

  uint16_t PacketType;
private:
  uint32_t map_id_;
};
