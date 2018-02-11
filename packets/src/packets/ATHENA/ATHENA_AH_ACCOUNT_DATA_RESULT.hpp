template <typename PacketSet>
struct type<PacketSet, ATHENA_AH_ACCOUNT_DATA_RESULT> {
  using packet_set = PacketSet;
  using packet_name = ATHENA_AH_ACCOUNT_DATA_RESULT;

  void emplace(const uint32_t aid,
               const char* email,
               const size_t email_sz,
               const uint32_t expiration_time,
               const uint8_t gmlevel) {
    PacketType = packet_sets::id_of<PacketSet, packet_name>::value;
    aid_ = aid;
    expiration_time_ = expiration_time;
    gmlevel_ = gmlevel;
    copy_buf_with_zero_pad(email_, sizeof(email_), email, email_sz);
  }

  void emplace(const uint32_t aid,
               const char* email,
               const size_t email_sz,
               const uint32_t expiration_time,
               const uint8_t gmlevel,
               const char* birthdate,
               const size_t birthdate_sz,
               const char* pin,
               const size_t pin_sz) {
    emplace(aid, email, email_sz, expiration_time, gmlevel);
    copy_buf_with_zero_pad(birthdate_, sizeof(birthdate_), birthdate, birthdate_sz);
    copy_buf_with_zero_pad(pin_, sizeof(pin_), pin, pin_sz);
  }

  void emplace(const uint32_t aid,
               const std::string& email,
               const std::optional<std::chrono::system_clock::time_point>& expiration_time,
               const uint8_t gmlevel,
               const std::optional<std::chrono::system_clock::time_point>& birthdate,
               const std::optional<std::string>& pin) {
    emplace(aid, email.c_str(), email.size() + 1, 0, gmlevel);
    if (expiration_time) expiration_time_ = to_host_epoch_time(*expiration_time);
    if (birthdate) {
      auto bd = to_gmdate_string(*birthdate);
      copy_buf_with_zero_pad(birthdate_, sizeof(birthdate_), bd.c_str(), bd.size() + 1);
    } else {
      memset(birthdate_, 0, sizeof(birthdate_));
    }
    if (pin) {
      copy_buf_with_zero_pad(pin_, sizeof(pin_), pin->c_str(), pin->size() + 1);
    } else {
      memset(pin_, 0, sizeof(pin_));
    }
  }
  
  explicit type(const uint32_t aid,
                const char* email,
                const size_t email_sz,
                const uint32_t expiration_time,
                const uint8_t gmlevel,
                const char* birthdate,
                const size_t birthdate_sz,
                const char* pin,
                const size_t pin_sz) {
    emplace(aid, email, email_sz, expiration_time, gmlevel, birthdate, birthdate_sz, pin, pin_sz);
  }

  explicit type(const uint32_t aid,
                const std::string& email,
                const std::optional<std::chrono::system_clock::time_point>& expiration_time,
                const uint8_t gmlevel,
                const std::optional<std::chrono::system_clock::time_point>& birthdate,
                const std::optional<std::string>& pin
                ) {
    emplace(aid, email, expiration_time, gmlevel, birthdate, pin);
  }
  
  explicit type(const uint32_t aid,
                const char* email,
                const size_t email_sz,
                const uint32_t expiration_time,
                const uint8_t gmlevel) {
    emplace(aid, email, email_sz, expiration_time, gmlevel);
  }

  inline uint32_t aid() const {
    return aid_;
  }

  inline const char* email() const {
    return email_;
  }

  inline uint32_t expiration_time() const {
    return expiration_time_;
  }

  inline uint8_t gmlevel() const {
    return gmlevel_;
  }

  inline const char* birthdate() const {
    return birthdate_;
  }

  inline const char* pin() const {
    return pin_;
  }
  
public:
  uint16_t PacketType;
private:
  uint32_t aid_;
  char email_[40];
  uint32_t expiration_time_;
  uint8_t gmlevel_;
  char birthdate_[11];
  char pin_[5];
};
