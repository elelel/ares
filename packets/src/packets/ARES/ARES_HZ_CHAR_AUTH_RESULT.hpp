template <typename PacketSet>
struct type<PacketSet, ARES_HZ_CHAR_AUTH_RESULT> {
  using packet_set = PacketSet;
  using packet_name = ARES_HZ_CHAR_AUTH_RESULT;

  inline void emplace(const uint32_t request_id,
                      const model::character_id& character_id,
                      const int32_t auth_code2,
                      const uint8_t result) {
    PacketType = packet_sets::id_of<PacketSet, packet_name>::value;
    request_id_ = request_id;
    character_id_ = character_id;
    auth_code2_ = auth_code2;
    result_ = result;
  }

  explicit inline type(const uint32_t request_id,
                       const model::character_id& character_id,
                       const uint32_t auth_code2,
                       const uint8_t result) {
    emplace(request_id, character_id, auth_code2, result);
  }

  inline const model::character_id& character_id() const {
    return character_id_;
  }

  inline int32_t request_id() const {
    return request_id_;
  }

  inline uint8_t result() const {
    return result_;
  }

  inline int32_t auth_code2() const {
    return auth_code2;
  }

  uint16_t PacketType;
private:
  uint32_t request_id_;
  model::character_id character_id_;
  int32_t auth_code2_;
  uint8_t result_;
};
