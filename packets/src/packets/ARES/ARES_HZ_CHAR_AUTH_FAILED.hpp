template <typename PacketSet>
struct type<PacketSet, ARES_HZ_CHAR_AUTH_FAILED> {
  using packet_set = PacketSet;
  using packet_name = ARES_HZ_CHAR_AUTH_FAILED;

  inline void emplace(const model::account_id& account_id,
                      const model::character_id& character_id,
                      const int32_t auth_code1,
                      const uint8_t sex,
                      const uint32_t ip) {
    PacketType = packet_sets::id_of<packet_set, packet_name>::value;
    account_id_ = account_id;
    character_id_ = character_id;
    auth_code1_ = auth_code1;
    sex_ = sex;
    ip_ = ip;
  }

  inline explicit type(const model::account_id& account_id,
                       const model::character_id& character_id,
                       const int32_t auth_code1,
                       const uint8_t sex,
                       const uint32_t ip) {
    emplace(account_id, character_id, auth_code1, sex, ip);
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

  inline uint8_t sex() const {
    return sex_;
  }

  inline uint32_t ip() const {
    return ip_;
  }

  uint16_t PacketType;
private:
  model::account_id account_id_;
  model::character_id character_id_;
  int32_t auth_code1_;
  uint8_t sex_;
  uint32_t ip_;  
};
