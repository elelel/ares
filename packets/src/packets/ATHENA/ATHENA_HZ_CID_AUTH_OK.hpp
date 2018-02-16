template <typename PacketSet>
struct type<PacketSet, ATHENA_HZ_CID_AUTH_OK> {
  using packet_set = PacketSet;
  using packet_name = ATHENA_HZ_CID_AUTH_OK;

  inline void emplace(const uint32_t aid,
                      const uint32_t cid,
                      const int32_t auth_code1,
                      const uint8_t sex,
                      const uint32_t ip) {
    PacketType = packet_sets::id_of<packet_set, packet_name>::value;
    aid_ = aid;
    cid_ = cid;
    auth_code1_ = auth_code1;
    sex_ = sex;
    ip_ = ip;
  }

  inline explicit type(const uint32_t aid,
                       const uint32_t cid,
                       const int32_t auth_code1,
                       const uint8_t sex,
                       const uint32_t ip) {
    emplace(aid, cid, auth_code1, sex, ip);
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

  inline const& character_info char_info() const {
    return char_info();
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
  character_info char_info_;
};
