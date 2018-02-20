template <typename PacketSet>
struct type<PacketSet, ARES_HA_AID_AUTH_REQ> {
  using packet_set = PacketSet;
  using packet_name = ARES_HA_AID_AUTH_REQ;

  inline void emplace(const uint32_t request_id,
                      const uint32_t aid,
                      const int32_t auth_code1,
                      const int32_t auth_code2) {
    PacketType = packet_sets::id_of<PacketSet, packet_name>::value;
    aid_ = aid;
    auth_code1_ = auth_code1;
    auth_code2_ = auth_code2;
    request_id_ = request_id;
  }

  inline explicit type(const uint32_t request_id,
                       const uint32_t aid,
                       const int32_t auth_code1,
                       const int32_t auth_code2) {
    emplace(request_id, aid, auth_code1, auth_code2);
  }
  
  inline int32_t request_id() const {
    return request_id_;
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

  uint16_t PacketType;
private:
  uint32_t request_id_;
  uint32_t aid_;
  int32_t auth_code1_;
  int32_t auth_code2_;

};
