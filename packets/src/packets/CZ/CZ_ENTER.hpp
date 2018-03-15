template <typename PacketSet>
struct type<PacketSet, CZ_ENTER> {
  using packet_set = PacketSet;
  using packet_name = CZ_ENTER;
  
  inline void emplace(const model::AID& AID,
                      const model::GID& GID,
                      const int32_t AuthCode,
                      const uint32_t clientTime,
                      const uint8_t Sex) {
    PacketType = packet_sets::id_of<packet_set, packet_name>::value;
    AID_ = AID;
    GID_ = GID;
    AuthCode_ = AuthCode;
    clientTime_ = clientTime;
    Sex_ = Sex;
  }

  inline explicit type(const model::AID& AID,
                       const model::GID& GID,
                       const int32_t AuthCode,
                       const uint32_t clientTime,
                       const uint8_t Sex) {
    emplace(AID, GID, AuthCode, clientTime, Sex);
  }

  inline const model::AID& AID() const {
    return AID_;
  }
  
  inline const model::GID& GID() const {
    return GID_;
  }
  
  inline int32_t AuthCode() const {
    return AuthCode_;
  }
  
  inline uint32_t clientTime() const {
    return clientTime_;
  }

  inline uint8_t Sex() const {
    return Sex_;
  }
  
  uint16_t PacketType;
private:
  model::AID AID_;
  model::GID GID_;
  int32_t AuthCode_;
  uint32_t clientTime_;
  uint8_t Sex_;
};
