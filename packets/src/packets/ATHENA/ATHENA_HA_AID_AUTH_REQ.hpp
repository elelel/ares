template <typename PacketSet>
struct type<PacketSet, ATHENA_HA_AID_AUTH_REQ> {
  using packet_set = PacketSet;
  using packet_name = ATHENA_HA_AID_AUTH_REQ;
  
  inline void emplace(const uint32_t aid,
                      const int32_t auth_code1,
                      const int32_t auth_code2,
                      const uint8_t sex,
                      const uint32_t ip,
                      const int32_t request_id) {
    PacketType = packet_sets::id_of<PacketSet, packet_name>::value;
    aid_ = aid;
    auth_code1_ = auth_code1;
    auth_code2_ = auth_code2;
    sex_ = sex;
    ip_ = ip;
    request_id_ = request_id;
  }

  inline explicit type(const uint32_t aid,
                const int32_t auth_code1,
                const int32_t auth_code2,
                const uint8_t sex,
                const uint32_t ip,
                const int32_t request_id) {
    emplace(aid, auth_code1, auth_code2, sex, ip, request_id);
  }

  inline uint32_t aid() const {
    return aid_;
  }

  inline int32_t auth_code1() const {
    return auth_code1_;
  }

  inline int32_t auth_code2() const {
    return auth_code2_;
  }

  inline uint8_t sex() const {
    return sex_;
  }

  inline uint32_t ip() const {
    return ip_;
  }

  inline int32_t request_id() const {
    return request_id_;
  }

  uint16_t PacketType;
private:
  model::account_id account_id_;
  int32_t auth_code1_;
  int32_t auth_code2_;
  uint8_t sex_;
  uint32_t ip_;
  int32_t request_id_;
};
