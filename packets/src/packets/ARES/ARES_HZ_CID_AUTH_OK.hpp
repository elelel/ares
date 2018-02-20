template <typename PacketSet>
struct type<PacketSet, ARES_HZ_CID_AUTH_OK> {
  using packet_set = PacketSet;
  using packet_name = ARES_HZ_CID_AUTH_OK;

  inline void emplace(const uint32_t aid,
                      const int32_t auth_code1,
                      const int32_t auth_code2,
                      const uint32_t expiration_time,
                      const uint32_t group_id,
                      const uint8_t changing_zoneservers,
                      const model::pc_info& pc) {
    PacketType = packet_sets::id_of<packet_set, packet_name>::value;
    PacketLength = sizeof(*this);
    aid_ = aid;
    auth_code1_ = auth_code1;
    auth_code2_ = auth_code2;
    expiration_time_ = expiration_time;
    group_id_ = group_id;
    changing_zoneservers_ = changing_zoneservers;
    pc_ = pc;
  }

  inline explicit type(const uint32_t aid,
                       const int32_t auth_code1,
                       const int32_t auth_code2,
                       const uint32_t expiration_time,
                       const uint32_t group_id,
                       const uint8_t changing_zoneservers,
                       const model::pc_info& pc) {
    emplace(aid, auth_code1, auth_code2, expiration_time, group_id, changing_zoneservers, pc);
  }
  
  inline uint32_t aid() const {
    return aid_;
  }

  inline int32_t auth_code1() const {
    return auth_code1_;
  }

  inline int32_t auth_code2() const {
    return auth_code1_;
  }

  inline uint32_t expiration_time() const {
    return expiration_time_;
  }

  inline uint32_t group_id() const {
    return group_id_;
  }

  inline uint8_t changing_zoneservers() const {
    return changing_zoneservers_;
  }

  inline const model::pc_info& pc() const {
    return pc();
  }
  
  uint16_t PacketType;
  uint16_t PacketLength;
private:
  uint32_t aid_;
  int32_t auth_code1_;
  int32_t auth_code2_;
  uint32_t expiration_time_;
  uint32_t group_id_;
  uint8_t changing_zoneservers_;
  model::pc_info pc_;
};
