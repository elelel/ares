static const size_t MAX_CHAR_SERVERS = 30;
static const size_t TWITTER_TOKEN_SIZE = 128;

template <typename PacketSet>
struct type<PacketSet, AC_ACCEPT_LOGIN> {
  using packet_set = PacketSet;
  using packet_name = AC_ACCEPT_LOGIN;
  
  inline void emplace(const int32_t AuthCode,
                      const model::AID AID,
                      const uint32_t userLevel,
                      const uint32_t lastLoginIP,
                      const std::string& lastLoginTime,
                      const uint8_t Sex,
                      const size_t servers_count) {
    PacketType = packet_sets::id_of<packet_set, packet_name>::value;
    PacketLength = sizeof(*this) + servers_count * sizeof(SERVER_ADDR);
    AuthCode_ = AuthCode;
    AID_ = AID;
    userLevel_ = userLevel;
    lastLoginIP_ = lastLoginIP;
    lastLoginTime_ = lastLoginTime;
    Sex_ = Sex;
    someNewByte_ = 0;
    memset(someNewBuf_, 0, sizeof(someNewBuf_));
  }

  inline explicit type(const int32_t AuthCode,
                       const model::AID AID,
                       const uint32_t userLevel,
                       const uint32_t lastLoginIP,
                       const std::string& lastLoginTime,
                       const uint8_t Sex,
                       const size_t servers_count) {
    emplace(AuthCode, AID, userLevel, lastLoginIP, lastLoginTime, Sex, servers_count);
  }
  
  struct SERVER_ADDR {
    void emplace(const uint32_t ip,
                 const uint16_t port,
                 const std::string& name,
                 const uint16_t user_count,
                 const uint16_t state,
                 const uint16_t property) {
      ip_ = ip;
      port_ = port;
      name_ = name;
      usercount_ = user_count;
      state_ = state;
      property_ = property;
      memset(&twitter_token_, 0, TWITTER_TOKEN_SIZE);
    }

    explicit SERVER_ADDR() {
    }
      
    explicit SERVER_ADDR(const uint32_t ip,
                         const uint16_t port,
                         const std::string& name,
                         const uint16_t user_count,
                         const uint16_t state,
                         const uint16_t property) {
      emplace(ip, port, name, user_count, state, property);
    }
    
  private:
    uint32_t ip_;
    uint16_t port_;
    model::fixed_string<20> name_;
    uint16_t usercount_;
    uint16_t state_;
    uint16_t property_;
    uint8_t twitter_token_[TWITTER_TOKEN_SIZE];
  };

  inline int32_t AuthCode() const {
    return AuthCode_;
  }

  inline model::AID AID() const {
    return AID_;
  }

  inline uint32_t userLevel() const {
    return userLevel_;
  }

  inline uint32_t lastLoginIP() const {
    return lastLoginIP_;
  }

  inline const model::fixed_string<26>& lastLoginTime() const {
    return lastLoginTime_;
  }

  inline uint8_t Sex() const {
    return Sex_;
  }

  inline const char* someNewBuf() const {
    return someNewBuf_;
  }

  inline uint8_t someNewByte() const {
    return someNewByte_;
  }

  inline const SERVER_ADDR* ServerList() const {
    return ServerList_;
  }
  
  uint16_t PacketType;
  uint16_t PacketLength;
  
private:
  int32_t AuthCode_;
  model::AID AID_;
  uint32_t userLevel_;
  uint32_t lastLoginIP_;
  model::fixed_string<26> lastLoginTime_;
  uint8_t Sex_;
  char someNewBuf_[16];
  uint8_t someNewByte_;
  SERVER_ADDR ServerList_[];
};
