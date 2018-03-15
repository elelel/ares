template <typename PacketSet>
struct type<PacketSet, ARES_ZH_CHAR_AUTH_REQ> {
  using packet_set = PacketSet;
  using packet_name = ARES_ZH_CHAR_AUTH_REQ;

  inline void emplace(const uint32_t request_id,
                      const model::account_id& account_id,
                      const model::character_id& character_id,
                      const int32_t auth_code1) {
    PacketType = packet_sets::id_of<PacketSet, packet_name>::value;
    account_id_ = account_id;
    character_id_ = character_id;
    auth_code1_ = auth_code1;
    request_id_ = request_id;
  }

  inline explicit type(const uint32_t request_id,
                       const model::account_id& account_id,
                       const model::character_id& character_id,
                       const int32_t auth_code1) {
    emplace(request_id, account_id, character_id, auth_code1);
  }
  
  inline int32_t request_id() const {
    return request_id_;
  }

  inline const model::account_id& account_id() const {
    return account_id_;
  }
  
  inline const model::character_id& character_id() const {
    return character_id_;
  }

  inline int32_t auth_code1() const {
    return auth_code1_;
  }

  uint16_t PacketType;
private:
  uint32_t request_id_;
  model::account_id account_id_;
  model::character_id character_id_;
  int32_t auth_code1_;
};
