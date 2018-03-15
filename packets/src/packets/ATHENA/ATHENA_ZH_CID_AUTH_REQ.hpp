template <typename PacketSet>
struct type<PacketSet, ATHENA_ZH_CID_AUTH_REQ> {
  using packet_set = PacketSet;
  using packet_name = ATHENA_ZH_CID_AUTH_REQ;

  inline void emplace(const uint32_t aid,
                      const uint32_t cid,
                      const int32_t auth_code1,
                      const uint8_t sex,
                      const uint32_t ip,
                      const uint8_t autotrade,
                      const uint32_t request_id) {
    PacketType = packet_sets::id_of<packet_set, packet_name>::value;
    aid_ = aid;
    cid_ = cid;
    auth_code1_ = auth_code1;
    sex_ = sex;
    ip_ = ip;
    autotrade_ = autotrade;
  }

  inline explicit type(const uint32_t aid,
                       const uint32_t cid,
                       const int32_t auth_code1,
                       const uint8_t sex,
                       const uint32_t ip,
                       const uint8_t autotrade,
                       const uint32_t request_id) {
    emplace(aid, cid, auth_code1, sex, ip, autotrade);
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

  inline uint8_t autotrade() const {
    return autotrade_;
  }
  
  uint16_t PacketType;
private:
  model::account_id account_id_;
  uint32_t cid_;
  int32_t auth_code1_;
  uint8_t sex_;
  uint32_t ip_;
  uint8_t autotrade_;
  uint32_t request_id;
};
