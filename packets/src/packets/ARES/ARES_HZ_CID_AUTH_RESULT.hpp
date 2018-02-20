template <typename PacketSet>
struct type<PacketSet, ARES_HZ_CID_AUTH_RESULT> {
  using packet_set = PacketSet;
  using packet_name = ARES_HZ_CID_AUTH_RESULT;

  void emplace(const uint32_t request_id,
               const uint32_t cid,
               const uint8_t result) {
    PacketType = packet_sets::id_of<PacketSet, packet_name>::value;
    request_id_ = request_id;
    cid_ = cid;
    result_ = result;
  }

  inline uint32_t cid() const {
    return cid_;
  }

  inline int32_t request_id() const {
    return request_id_;
  }

  inline uint8_t result() const {
    return result_;
  }

  uint16_t PacketType;
private:
  uint32_t request_id_;
  uint32_t cid_;
  uint8_t result_;
};
