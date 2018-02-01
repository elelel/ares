template <typename PacketSet>
struct type<PacketSet, ATHENA_HA_LOGIN_REQ> {
  using packet_name = ATHENA_HA_LOGIN_REQ;
  
  void emplace(const char* login,
               const size_t login_sz,
               const char* password,
               const size_t password_sz,
               const uint32_t dummy1,
               const uint32_t ip,
               const uint16_t port,
               const char* server_name,
               const size_t server_name_sz,
               const uint16_t state,
               const uint16_t property,
               const uint16_t dummy2) {
    PacketType = PacketSet::template id_of<type<PacketSet, packet_name>>::value;
    dummy1_ = dummy1;
    ip_ = ip;
    port_ = port;
    state_ = state;
    property_ = property;
    dummy2_ = dummy2;
    copy_buf_with_zero_pad(this->login_, sizeof(this->login_), login, login_sz);
    copy_buf_with_zero_pad(this->password_, sizeof(this->password_), password, password_sz);
    copy_buf_with_zero_pad(this->server_name_, sizeof(this->server_name_), server_name, server_name_sz);
  }

  void emplace(const std::string& login,
               const std::string& password,
               const uint32_t ip,
               const uint16_t port,
               const std::string& server_name,
               const uint16_t state,
               const uint16_t property) {
    emplace(login.c_str(), login.size(), password.c_str(), password.size(),
            0, ip, port, server_name.c_str(), server_name.size(), state, property, 0);
  }               

  explicit type(const char* login,
                const size_t login_sz,
                const char* password,
                const size_t password_sz,
                const uint32_t dummy1,
                const uint32_t ip,
                const uint16_t port,
                const char* server_name,
                const size_t server_name_sz,
                const uint16_t state,
                const uint16_t property,
                const uint16_t dummy2) {
    emplace(login, login_sz, password, password_sz, dummy1, ip, port, server_name, server_name_sz, state, property, dummy2);
  }

  explicit type(const std::string& login,
                const std::string& password,
                const uint32_t ip,
                const uint16_t port,
                const std::string& server_name,
                const uint16_t state,
                const uint16_t property) {
    emplace(login, password, ip, port, server_name, state, property);
  }
  
  const char* login() const {
    return login_;
  }

  const char* password() const {
    return password_;
  }

  uint32_t ip() const {
    return ip_;
  }

  uint16_t port() const {
    return port_;
  }

  const char* server_name() const {
    return server_name_;
  }

  uint16_t state() const {
    return state_;
  }

  uint16_t property() const {
    return property_;
  }
  
  uint16_t PacketType;
private:
  char login_[24];
  char password_[24];
  uint32_t dummy1_;
  uint32_t ip_;
  uint16_t port_;
  char server_name_[20];
  uint16_t state_;
  uint16_t property_;
  uint16_t dummy2_;
};
