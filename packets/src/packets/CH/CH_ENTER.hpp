template <typename PacketSet>
struct type<PacketSet, CH_ENTER> {
  using packet_set = PacketSet;
  using packet_name = CH_ENTER;
  
  inline void emplace(const model::AID& AID,
                      const int32_t AuthCode,
                      const int32_t userLevel,
                      const uint16_t clientType,
                      const uint8_t Sex) {
    PacketType = packet_sets::id_of<packet_set, packet_name>::value;
    AID_ = AID;
    AuthCode_ = AuthCode;
    userLevel_ = userLevel;
    clientType_ = clientType;
    Sex_ = Sex;
  }

  inline explicit type(const model::AID& AID,
                       const int32_t AuthCode,
                       const int32_t userLevel,
                       const uint16_t clientType,
                       const uint8_t Sex) {
    emplace(AID, AuthCode, userLevel, clientType, Sex);
  }

  inline const model::AID& AID() const {
    return AID_;
  }

  inline int32_t AuthCode() const {
    return AuthCode_;
  }

  inline int32_t userLevel() const {
    return userLevel_;
  }

  inline uint16_t clientType() const {
    return clientType_;
  }

  inline uint8_t Sex() const {
    return Sex_;
  }
  
  uint16_t PacketType;
private:
  model::AID AID_;
  int32_t AuthCode_;
  int32_t userLevel_;
  uint16_t clientType_;
  uint8_t Sex_;
};
