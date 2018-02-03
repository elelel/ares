template <typename PacketSet>
struct type<PacketSet, CZ_ENTER> {
  using packet_name = CZ_ENTER;
  
  inline void emplace(const uint32_t AID,
                      const uint32_t GID,
                      const int32_t AuthCode,
                      const uint32_t clientTime,
                      const uint8_t Sex) {
    PacketType = packet_sets::id_of<PacketSet, packet_name>::value;
    AID_ = AID;
    GID_ = GID;
    AuthCode_ = AuthCode;
    clientTime_ = clientTime;
    Sex_ = Sex;
  }

  explicit type<PacketSet, CZ_ENTER>(const uint32_t AID,
                      const uint32_t GID,
                      const int32_t AuthCode,
                      const uint32_t clientTime,
                      const uint8_t Sex) {
    emplace(AID, GID, AuthCode, clientTime, Sex);
  }

  uint32_t AID() const {
    return AID_;
  }
  
  uint32_t GID() const {
    return GID_;
  }
  
  int32_t AuthCode() const {
    return AuthCode_;
  }
  
  uint32_t clientTime() const {
    return clientTime_;
  }

  uint8_t Sex() const {
    return Sex_;
  }
  
  uint16_t PacketType;
private:
  uint32_t AID_;
  uint32_t GID_;
  int32_t AuthCode_;
  uint32_t clientTime_;
  uint8_t Sex_;
};
