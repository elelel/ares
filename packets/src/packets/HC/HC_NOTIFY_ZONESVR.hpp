template <typename PacketSet>
struct type<PacketSet, HC_NOTIFY_ZONESVR> {
  using packet_set = PacketSet;
  using packet_name = HC_NOTIFY_ZONESVR;
 
  inline void emplace(const model::GID& GID,
                      const std::string& mapName,
                      const uint32_t ip,
                      const uint16_t port) {
    PacketType = packet_sets::id_of<packet_set, packet_name>::value;
    GID_ = GID;
    mapName_ = mapName;
    ip_ = ip;
    port_ = port;
  }

  explicit inline type(const model::GID& GID,
                       const std::string& mapName,
                       const uint32_t ip,
                       const uint16_t port) {
    emplace(GID, mapName, ip, port);
  }
  
  inline const model::GID& GID() const {
    return GID_;
  }

  inline const model::map_name_ext_string& mapName() const {
    return mapName_;
  }

  inline uint32_t ip() const {
    return ip_;
  }

  inline uint16_t port() const {
    return port_;
  }
  
  uint16_t PacketType;
private:
  model::GID GID_;
  model::map_name_ext_string mapName_;
  uint32_t ip_;
  uint16_t port_;
};
