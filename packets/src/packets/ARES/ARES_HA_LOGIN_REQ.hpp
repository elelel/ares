template <typename PacketSet>
struct type<PacketSet, ARES_HA_LOGIN_REQ> {
  using packet_set = PacketSet;
  using packet_name = ARES_HA_LOGIN_REQ;
  
  inline void emplace(const std::string& login,
                      const std::string& password,
                      const uint32_t ip,
                      const uint16_t port,
                      const std::string& server_name,
                      const uint16_t state,
                      const uint16_t property) {
    PacketType = packet_sets::id_of<PacketSet, packet_name>::value;
    login_ = login;
    password_ = password;
    ip_ = ip;
    port_ = port;
    state_ = state;
    server_name_ = server_name;
    property_ = property;
  }               

  explicit inline type(const std::string& login,
                       const std::string& password,
                       const uint32_t ip,
                       const uint16_t port,
                       const std::string& server_name,
                       const uint16_t state,
                       const uint16_t property) {
    emplace(login, password, ip, port, server_name, state, property);
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

  const model::fixed_string<20>& server_name() const {
    return server_name_;
  }

  inline uint16_t state() const {
    return state_;
  }

  inline uint16_t property() const {
    return property_;
  }
  
  uint16_t PacketType;
private:
  model::fixed_string<24> login_;
  model::fixed_string<24> password_;
  uint32_t ip_;
  uint16_t port_;
  model::fixed_string<20> server_name_;
  uint16_t state_;
  uint16_t property_;
};
