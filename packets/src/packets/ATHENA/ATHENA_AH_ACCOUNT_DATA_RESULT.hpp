template <typename PacketSet>
struct type<PacketSet, ATHENA_AH_ACCOUNT_DATA_RESULT> {
  using packet_set = PacketSet;
  using packet_name = ATHENA_AH_ACCOUNT_DATA_RESULT;

  inline void emplace(const uint32_t aid,
                      const std::string& email,
                      const std::optional<std::chrono::system_clock::time_point>& expiration_time,
                      const uint8_t gmlevel,
                      const std::optional<std::chrono::system_clock::time_point>& birthdate,
                      const std::optional<std::string>& pin) {
    PacketType = packet_sets::id_of<PacketSet, packet_name>::value;
    aid_ = aid;
    gmlevel_ = gmlevel;
    email_ = email;
    
    if (expiration_time) {
      expiration_time_ = model::to_host_epoch_time(*expiration_time);
    } else {
      expiration_time_ = 0;
    }
    
    if (birthdate) {
      birthdate_ = model::to_gmdate_string(*birthdate);
    } else {
      birthdate_ = "";
    }
    
    if (pin) {
      pin_ = *pin;
    } else {
      pin_ = "";
    }
  }
  
  explicit inline type(const uint32_t aid,
                       const std::string& email,
                       const std::optional<std::chrono::system_clock::time_point>& expiration_time,
                       const uint8_t gmlevel,
                       const std::optional<std::chrono::system_clock::time_point>& birthdate,
                       const std::optional<std::string>& pin
                       ) {
    emplace(aid, email, expiration_time, gmlevel, birthdate, pin);
  }

  inline uint32_t aid() const {
    return aid_;
  }

  inline const model::fixed_string<40>& email() const {
    return email_;
  }

  inline uint32_t expiration_time() const {
    return expiration_time_;
  }

  inline uint8_t gmlevel() const {
    return gmlevel_;
  }

  inline const model::fixed_string<11>& birthdate() const {
    return birthdate_;
  }

  inline const model::fixed_string<5>& pin() const {
    return pin_;
  }
  
public:
  uint16_t PacketType;
private:
  uint32_t aid_;
  model::fixed_string<40> email_;
  uint32_t expiration_time_;
  uint8_t gmlevel_;
  model::fixed_string<11> birthdate_;
  model::fixed_string<5> pin_;
};
