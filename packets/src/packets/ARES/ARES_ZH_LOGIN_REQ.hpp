template <typename PacketSet>
struct type<PacketSet, ARES_ZH_LOGIN_REQ> {
  using packet_set = PacketSet;
  using packet_name = ARES_ZH_LOGIN_REQ;
  
  inline void emplace(const std::string& login,
                      const std::string& password,
                      const uint32_t ip,
                      const uint16_t port) {
    PacketType = packet_sets::id_of<packet_set, packet_name>::value;
    login_ = login;
    password_ = password;
    ip_ = ip;
    port_ = port;
  }
  
  explicit inline type(const std::string& login,
                       const std::string& password,
                       const uint32_t ip,
                       const uint16_t port) {
    emplace(login, password, ip, port);
  }

  inline const model::fixed_string<24>& login() const {
    return login_;
  }

  inline const model::fixed_string<24>& password() const {
    return password_;
  }

  inline uint32_t ip() const {
    return ip_;
  }

  inline uint16_t port() const {
    return port_;
  }
  
  uint16_t PacketType;
private:
  model::fixed_string<24> login_;
  model::fixed_string<24> password_;
  uint32_t ip_;
  uint16_t port_;
};
