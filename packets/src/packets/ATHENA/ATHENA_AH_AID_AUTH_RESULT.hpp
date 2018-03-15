template <typename PacketSet>
struct type<PacketSet, ATHENA_AH_AID_AUTH_RESULT> {
  using packet_set = PacketSet;
  using packet_name = ATHENA_AH_AID_AUTH_RESULT;
  
  void emplace(const uint32_t aid,
               const int32_t auth_code1,
               const int32_t auth_code2,
               const uint8_t sex,
               const uint8_t fail,
               const int32_t request_id,
               const uint32_t version,
               const uint8_t clienttype) {
    PacketType = packet_sets::id_of<PacketSet, packet_name>::value;
    aid_ = aid;
    auth_code1_ = auth_code1;
    auth_code2_ = auth_code2;
    sex_ = sex;
    fail_ = fail;
    request_id_ = request_id;
    version_ = version;
    clienttype_ = clienttype;
  }
  
  explicit type(const uint32_t aid,
                const int32_t auth_code1,
                const int32_t auth_code2,
                const uint8_t sex,
                const uint8_t fail,
                const int32_t request_id,
                const uint32_t version,
                const uint8_t clienttype) {
    emplace(aid, auth_code1, auth_code2, sex, fail, request_id, version, clienttype);
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
  
  inline uint8_t fail() const {
    return fail_;
  }

  inline int32_t request_id() const {
    return request_id_;
  }
  
  inline uint32_t version() const {
    return version_;
  }
  
  inline uint8_t clienttype() const {
    return clienttype_;
  }
  
  uint16_t PacketType;
private:
  model::account_id account_id_;
  int32_t auth_code1_;
  int32_t auth_code2_;
  uint8_t sex_;
  uint8_t fail_;
  int32_t request_id_;
  uint32_t version_;
  uint8_t clienttype_;
};
