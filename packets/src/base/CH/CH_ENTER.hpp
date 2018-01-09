struct CH_ENTER {
  inline void emplace(const uint32_t AID,
               const int32_t AuthCode,
               const uint32_t userLevel,
               const uint16_t clientType,
               const uint8_t Sex) {
    PacketType = packet_id::CH_ENTER;
    AID_ = AID;
    AuthCode_ = AuthCode;
    userLevel_ = userLevel;
    clientType_ = clientType;
    Sex_ = Sex;
  }

  explicit CH_ENTER(const uint32_t AID,
                    const int32_t AuthCode,
                    const uint32_t userLevel,
                    const uint16_t clientType,
                    const uint8_t Sex) {
    emplace(AID, AuthCode, userLevel, clientType, Sex);
  }

  inline uint32_t AID() const {
    return AID_;
  }

  inline int32_t AuthCode() const {
    return AuthCode_;
  }

  inline uint32_t userLevel() const {
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
  uint32_t AID_;
  int32_t AuthCode_;
  uint32_t userLevel_;
  uint16_t clientType_;
  uint8_t Sex_;
};
