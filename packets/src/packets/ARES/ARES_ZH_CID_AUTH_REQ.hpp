template <typename PacketSet>
struct type<PacketSet, ARES_ZH_CID_AUTH_REQ> {
  using packet_set = PacketSet;
  using packet_name = ARES_ZH_CID_AUTH_REQ;

  inline void emplace(const uint32_t request_id,
                      const uint32_t aid,
                      const uint32_t cid,
                      const int32_t auth_code1) {
    PacketType = packet_sets::id_of<PacketSet, packet_name>::value;
    aid_ = aid;
    cid_ = cid;
    auth_code1_ = auth_code1;
    request_id_ = request_id;
  }

  inline explicit type(const uint32_t request_id,
                       const uint32_t aid,
                       const uint32_t cid,
                       const int32_t auth_code1) {
    emplace(request_id, aid, cid, auth_code1);
  }
  
  inline int32_t request_id() const {
    return request_id_;
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

  uint16_t PacketType;
private:
  uint32_t request_id_;
  uint32_t aid_;
  uint32_t cid_;
  int32_t auth_code1_;
};
