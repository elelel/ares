struct CA_SSO_LOGIN_REQ {
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
    PacketType = packet_id::CA_SSO_LOGIN_REQ;
    PacketLength = sizeof(CA_SSO_LOGIN_REQ) + token_sz;
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

  explicit CA_SSO_LOGIN_REQ(const uint32_t Version,
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

  explicit CA_SSO_LOGIN_REQ(const uint32_t Version,
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

  uint16_t PacketType;
  uint16_t PacketLength;
private:
  uint32_t Version_;
  uint8_t clienttype_;
public: // To allow sizeof
  char ID_[24];
  char Passwd_[27];
private:  
  char MacAddress_[17];
  char IP_[15];
  char token_[];
};
