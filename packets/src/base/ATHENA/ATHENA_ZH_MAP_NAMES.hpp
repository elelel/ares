struct ATHENA_ZH_MAP_NAMES {
  void emplace(const size_t num_maps) {
    PacketType = packet_id::ATHENA_ZH_MAP_NAMES;
    PacketLength = sizeof(ATHENA_ZH_MAP_NAMES) + num_maps * sizeof(uint32_t);
  }

  explicit ATHENA_ZH_MAP_NAMES(const size_t num_maps) {
    emplace(num_maps);
  }

  const uint32_t* map_ids() const {
    return map_ids_;
  }
  
  uint16_t PacketType;
  uint16_t PacketLength;
private:
  uint32_t map_ids_[];
};
