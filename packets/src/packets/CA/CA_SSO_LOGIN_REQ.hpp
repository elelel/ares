template <typename PacketSet>
struct type<PacketSet, CA_SSO_LOGIN_REQ::login_password> {
  using packet_set = PacketSet;
  using packet_name = CA_SSO_LOGIN_REQ::login_password;
  
  void emplace(const uint32_t Version,
               const std::string& ID,
               const std::string& Passwd,
               const uint8_t clienttype) {
    PacketType = packet_sets::id_of<PacketSet, packet_name>::value;
    Version_ = Version;
    clienttype_ = clienttype;
    ID_ = ID;
    Passwd_ = Passwd;
  }

  explicit type(const uint32_t Version,
                const std::string& ID,
                const std::string& Passwd,
                const uint8_t clienttype) {
    emplace(Version, ID, Passwd, clienttype);
  }
  
  uint32_t Version() const {
    return Version_;
  }

  const model::login_string& ID() const {
    return ID_;
  }

  const model::fixed_string<24> Passwd() const {
    return Passwd_;
  }

  uint8_t clienttype() const {
    return clienttype_;
  }

  uint16_t PacketType;
private:
  uint32_t Version_;
  model::login_string ID_;
  model::fixed_string<24> Passwd_;
  uint8_t clienttype_;
};

template <typename PacketSet>
struct type<PacketSet, CA_SSO_LOGIN_REQ::token_auth> {
  using packet_set = PacketSet;
  using packet_name = CA_SSO_LOGIN_REQ::token_auth;
  
  inline void emplace(const uint32_t Version,
               const uint8_t clienttype,
               const std::string& ID,
               const std::string& Passwd,
               const std::string& MacAddress,
               const std::string& IP,
               const size_t token_sz) {
    PacketType = packet_sets::id_of<PacketSet, packet_name>::value;
    PacketLength = sizeof(*this) + token_sz;
    Version_ = Version;
    clienttype_ = clienttype;

    ID_ = ID;
    Passwd_ = Passwd;
    MacAddress_ = MacAddress;
    IP_ = IP;
  }

  inline explicit type(const uint32_t Version,
                const uint8_t clienttype,
                const std::string& ID,
                const std::string& Passwd,
                const std::string& MacAddress,
                const std::string& IP,
                const size_t token_sz) {
    emplace(Version, clienttype, ID, Passwd, MacAddress, IP, token_sz);
  }
  
  inline uint32_t Version() const {
    return Version_;
  }

  inline uint8_t clienttype() const {
    return clienttype_;
  }

  inline const model::login_string& ID() const {
    return ID_;
  }

  inline const model::fixed_string<27> Passwd() const {
    return Passwd_;
  }

  inline const model::fixed_string<17> MacAddress() const {
    return MacAddress_;
  }

  inline const model::fixed_string<15> IP() const {
    return IP_;
  }

  inline const char* token() const {
    return token_;
  }

  uint16_t PacketType;
  uint16_t PacketLength;
private:
  uint32_t Version_;
  uint8_t clienttype_;
  model::login_string ID_;
  model::fixed_string<27> Passwd_;
  model::fixed_string<17> MacAddress_;
  model::fixed_string<15> IP_;
  char token_[];
};
