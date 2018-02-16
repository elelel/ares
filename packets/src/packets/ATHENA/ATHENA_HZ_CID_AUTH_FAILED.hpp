template <typename PacketSet>
struct type<PacketSet, ATHENA_HZ_CID_AUTH_FAILED> {
  using packet_set = PacketSet;
  using packet_name = ATHENA_HZ_CID_AUTH_FAILED;

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

  inline uint32_t cid() const {
    return cid_;
  }

  inline int32_t auth_code1() const {
    return auth_code1_;
  }

  inline uint8_t sex() const {
    return sex_;
  }

  inline uint32_t ip() const {
    return ip_;
  }

  uint16_t PacketType;
private:
  uint32_t aid_;
  uint32_t cid_;
  int32_t auth_code1_;
  uint8_t sex_;
  uint32_t ip_;  
};
