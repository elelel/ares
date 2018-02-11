template <typename PacketSet>
struct type<PacketSet, HC_NOTIFY_ZONESVR> {
  using packet_set = PacketSet;
  using packet_name = HC_NOTIFY_ZONESVR;
  
  inline void emplace(uint32_t GID,
                      const char* mapName,
                      size_t mapName_sz,
                      uint32_t ip,
                      uint16_t port) {
    PacketType = packet_sets::id_of<PacketSet, packet_name>::value;
    GID_ = GID;
    ip_ = ip;
    port_ = port;

    copy_buf_with_zero_pad(mapName_, sizeof(mapName_), mapName, mapName_sz);
  }

  inline void emplace(uint32_t GID,
                      const std::string& mapName,
                      uint32_t ip,
                      uint16_t port) {
    emplace(GID, mapName.c_str(), mapName.size(), ip, port);
  }

  explicit type(uint32_t GID,
                const char* mapName,
                size_t mapName_sz,
                uint32_t ip,
                uint16_t port) {
    emplace(GID, mapName, mapName_sz, ip, port);
  }

  explicit type(uint32_t GID,
                const std::string& mapName,
                uint32_t ip,
                uint16_t port) {
    emplace(GID, mapName, ip, port);
  }
  
  uint32_t GID() const {
    return GID_;
  }

  const char* mapName() const {
    return mapName_;
  }

  uint32_t ip() const {
    return ip_;
  }

  uint16_t port() const {
    return port_;
  }
  
  uint16_t PacketType;
private:
  uint32_t GID_;  // char id
  char mapName_[16];
  uint32_t ip_;
  uint16_t port_;
};
