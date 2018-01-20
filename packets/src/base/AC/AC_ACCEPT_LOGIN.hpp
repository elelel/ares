static const size_t MAX_CHAR_SERVERS = 30;
static const size_t TWITTER_TOKEN_SIZE = 128;

struct AC_ACCEPT_LOGIN {
  inline void emplace(const int32_t AuthCode,
                      const uint32_t AID,
                      const uint32_t userLevel,
                      const uint32_t lastLoginIP,
                      const char* lastLoginTime,
                      const size_t lastLoginTime_sz,
                      const uint8_t Sex,
                      const size_t servers_count) {
    PacketType = packet_id::AC_ACCEPT_LOGIN;
    PacketLength = sizeof(*this) + servers_count * sizeof(SERVER_ADDR);
    AuthCode_ = AuthCode;
    AID_ = AID;
    userLevel_ = userLevel;
    lastLoginIP_ = lastLoginIP;
    Sex_ = Sex;
    someNewByte_ = 0;
    copy_buf_with_zero_pad(lastLoginTime_, sizeof(lastLoginTime_), lastLoginTime, lastLoginTime_sz);
    memset(someNewBuf_, 0, sizeof(someNewBuf_));
  }

  inline void emplace(const int32_t AuthCode,
                      const uint32_t AID,
                      const uint32_t userLevel,
                      const uint32_t lastLoginIP,
                      const std::string& lastLoginTime,
                      const uint8_t Sex,
                      const size_t servers_count) {
    emplace(AuthCode, AID, userLevel, lastLoginIP, lastLoginTime.c_str(), lastLoginTime.size() + 1, Sex, servers_count);
  }

  explicit AC_ACCEPT_LOGIN(const int32_t AuthCode,
                           const uint32_t AID,
                           const uint32_t userLevel,
                           const uint32_t lastLoginIP,
                           const char* lastLoginTime,
                           const size_t lastLoginTime_sz,
                           const uint8_t Sex,
                           const size_t servers_count) {
    emplace(AuthCode, AID, userLevel, lastLoginIP, lastLoginTime, lastLoginTime_sz, Sex, servers_count);
  }

  explicit AC_ACCEPT_LOGIN(const int32_t AuthCode,
                           const uint32_t AID,
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
                 const char* name,
                 const uint32_t name_sz,
                 const uint16_t user_count,
                 const uint16_t state,
                 const uint16_t property) {
      ip_ = ip;
      port_ = port;
      usercount_ = user_count;
      state_ = state;
      property_ = property;
      copy_buf_with_zero_pad(name_, sizeof(name_), name, name_sz);
      memset(&twitter_token_, 0, TWITTER_TOKEN_SIZE);
    }

    void emplace(const uint32_t ip,
                 const uint16_t port,
                 const std::string& name,
                 const uint16_t user_count,
                 const uint16_t state,
                 const uint16_t property) {
      emplace(ip, port, name.c_str(), name.size() + 1, user_count, state, property);
    }
      
    explicit SERVER_ADDR(const uint32_t ip,
                         const uint16_t port,
                         const char* name,
                         const uint32_t name_sz,
                         const uint16_t user_count,
                         const uint16_t state,
                         const uint16_t property) {
      emplace(ip, port, name, name_sz, user_count, state, property);
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
    char name_[20];
    uint16_t usercount_;
    uint16_t state_;
    uint16_t property_;
    uint8_t twitter_token_[TWITTER_TOKEN_SIZE];
  };

  inline int32_t AuthCode() const {
    return AuthCode_;
  }

  inline uint32_t AID() const {
    return AID_;
  }

  inline uint32_t userLevel() const {
    return userLevel_;
  }

  inline uint32_t lastLoginIP() const {
    return lastLoginIP_;
  }

  inline const char* lastLoginTime() const {
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
  uint32_t AID_;
  uint32_t userLevel_;
  uint32_t lastLoginIP_;
  char lastLoginTime_[26];
  uint8_t Sex_;
  char someNewBuf_[16];
  uint8_t someNewByte_;
  SERVER_ADDR ServerList_[];
};
