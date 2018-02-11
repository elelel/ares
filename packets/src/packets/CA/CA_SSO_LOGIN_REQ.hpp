template <typename PacketSet>
struct type<PacketSet, CA_SSO_LOGIN_REQ::login_password> {
  using packet_set = PacketSet;
  using packet_name = CA_SSO_LOGIN_REQ::login_password;
  
  void emplace(const uint32_t Version,
               const char* ID,
               const size_t ID_sz,
               const char* Passwd,
               const size_t Passwd_sz,
               const uint8_t clienttype) {
    PacketType = packet_sets::id_of<PacketSet, packet_name>::value;
    Version_ = Version;
    clienttype_ = clienttype;
    copy_buf_with_zero_pad(ID_, sizeof(ID_), ID, ID_sz);
    copy_buf_with_zero_pad(Passwd_, sizeof(Passwd_), Passwd, Passwd_sz);
  }

  void emplace(const uint32_t Version,
               const std::string& ID,
               const std::string& Passwd,
               const uint8_t clienttype) {
    emplace(Version, ID.c_str(), ID.size(), Passwd.c_str(), Passwd.size(), clienttype);
  }

  explicit type(const uint32_t Version,
                const char* ID,
                const size_t ID_sz,
                const char* Passwd,
                const size_t Passwd_sz,
                const uint8_t clienttype) {
    emplace(Version, ID, ID_sz, Passwd, Passwd_sz, clienttype);
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

  const char* ID() const {
    return ID_;
  }

  const char* Passwd() const {
    return Passwd_;
  }

  uint8_t clienttype() const {
    return clienttype_;
  }

  constexpr static size_t ID_size() {
    return 24;
  }

  constexpr static size_t Passwd_size() {
    return 24;
  }

  uint16_t PacketType;
private:
  uint32_t Version_;
  char ID_[ID_size()];
  char Passwd_[Passwd_size()];
private:  
  uint8_t clienttype_;
};

template <typename PacketSet>
struct type<PacketSet, CA_SSO_LOGIN_REQ::token_auth> {
  using packet_set = PacketSet;
  using packet_name = CA_SSO_LOGIN_REQ::token_auth;
  
  void emplace(const uint32_t Version,
               const uint8_t clienttype,
               const char* ID,
               const size_t ID_sz,
               const char* Passwd,
               const size_t Passwd_sz,
               const char* MacAddress,
               const size_t MacAddress_sz,
               const char* IP,
               const size_t IP_sz,
               const size_t token_sz) {
    PacketType = packet_sets::id_of<PacketSet, packet_name>::value;
    PacketLength = sizeof(*this) + token_sz;
    Version_ = Version;
    clienttype_ = clienttype;
    copy_buf_with_zero_pad(ID_, sizeof(ID_), ID, ID_sz);
    copy_buf_with_zero_pad(Passwd_, sizeof(Passwd_), Passwd, Passwd_sz);
    copy_buf_with_zero_pad(MacAddress_, sizeof(MacAddress_), MacAddress, MacAddress_sz);
    copy_buf_with_zero_pad(IP_, sizeof(IP_), IP, IP_sz);
  }

  void emplace(const uint32_t Version,
               const uint8_t clienttype,
               const std::string& ID,
               const std::string& Passwd,
               const std::string& MacAddress,
               const std::string& IP,
               const size_t token_sz) {
    emplace(Version, clienttype, ID.c_str(), ID.size(), Passwd.c_str(), Passwd.size(),
            MacAddress.c_str(), MacAddress.size(), IP.c_str(), IP.size(), token_sz);
  }

  explicit type(const uint32_t Version,
                const uint8_t clienttype,
                const char* ID,
                const size_t ID_sz,
                const char* Passwd,
                const size_t Passwd_sz,
                const char* MacAddress,
                const size_t MacAddress_sz,
                const char* IP,
                const size_t IP_sz,
                const size_t token_sz) {
    emplace(Version, clienttype, ID, ID_sz, Passwd, Passwd_sz, MacAddress, MacAddress_sz, IP, IP_sz, token_sz);
  }

  explicit type(const uint32_t Version,
                const uint8_t clienttype,
                const std::string& ID,
                const std::string& Passwd,
                const std::string& MacAddress,
                const std::string& IP,
                const size_t token_sz) {
    emplace(Version, clienttype, ID, Passwd, MacAddress, IP, token_sz);
  }
  
  uint32_t Version() const {
    return Version_;
  }

  uint8_t clienttype() const {
    return clienttype_;
  }

  const char* ID() const {
    return ID_;
  }

  const char* Passwd() const {
    return Passwd_;
  }

  const char* MacAddress() const {
    return MacAddress_;
  }

  const char* IP() const {
    return IP_;
  }

  const char* token() const {
    return token_;
  }

  constexpr size_t ID_size() {
    return 24;
  }

  constexpr size_t Passwd_size() {
    return 24;
  }

  uint16_t PacketType;
  uint16_t PacketLength;
private:
  uint32_t Version_;
  uint8_t clienttype_;
  char ID_[24];
  char Passwd_[27];
private:  
  char MacAddress_[17];
  char IP_[15];
  char token_[];
};
