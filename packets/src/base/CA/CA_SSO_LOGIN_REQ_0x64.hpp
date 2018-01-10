struct CA_SSO_LOGIN_REQ_0x64 {
  void emplace(const uint32_t Version,
               const char* ID,
               const size_t ID_sz,
               const char* Passwd,
               const size_t Passwd_sz,
               const uint8_t clienttype) {
    PacketType = packet_id::CA_SSO_LOGIN_REQ_0x64;
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

  explicit CA_SSO_LOGIN_REQ_0x64(const uint32_t Version,
                            const char* ID,
                            const size_t ID_sz,
                            const char* Passwd,
                            const size_t Passwd_sz,
                            const uint8_t clienttype) {
    emplace(Version, ID, ID_sz, Passwd, Passwd_sz, clienttype);
  }

  explicit CA_SSO_LOGIN_REQ_0x64(const uint32_t Version,
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

  uint16_t PacketType;
private:
  uint32_t Version_;
public: // To allow sizeof
  char ID_[24];
  char Passwd_[24];
private:  
  uint8_t clienttype_;
};
