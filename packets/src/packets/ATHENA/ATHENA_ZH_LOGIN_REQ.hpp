template <typename PacketSet>
struct type<PacketSet, ATHENA_ZH_LOGIN_REQ> {
  using packet_name = ATHENA_ZH_LOGIN_REQ;
  
  void emplace(const char* login,
               const size_t login_sz,
               const char* password,
               const size_t password_sz,
               const uint32_t dummy,
               const uint32_t ip,
               const uint16_t port) {
    PacketType = PacketSet::template id_of<type<PacketSet, packet_name>>::value;
    dummy_ = dummy;
    ip_ = ip;
    port_ = port;
    copy_buf_with_zero_pad(this->login_, sizeof(this->login_), login, login_sz);
    copy_buf_with_zero_pad(this->password_, sizeof(this->password_), password, password_sz);
  }

  void emplace(const std::string& login,
               const std::string& password,
               const uint32_t dummy,
               const uint32_t ip,
               const uint16_t port) {
    emplace(login.c_str(), login.size(), password.c_str(), password.size(), dummy, ip, port);
  }

  explicit type(const char* login,
                const size_t login_sz,
                const char* password,
                const size_t password_sz,
                const uint32_t dummy,
                const uint32_t ip,
                const uint16_t port) {
    emplace(login, login_sz, password, password_sz, dummy, ip, port);
  }

  explicit type(const std::string& login,
                const std::string& password,
                const uint32_t dummy,
                const uint32_t ip,
                const uint16_t port) {
    emplace(login, password, dummy, ip, port);
  }

  const char* login() const {
    return login_;
  }

  const char* password() const {
    return password_;
  }

  uint32_t dummy() const {
    return dummy_;
  }

  uint32_t ip() const {
    return ip_;
  }

  uint16_t port() const {
    return port_;
  }
  
  uint16_t PacketType;
private:
  char login_[24];
  char password_[24];
  uint32_t dummy_;
  uint32_t ip_;
  uint16_t port_;
};
