struct ATHENA_ZH_USER_COUNT {
  void emplace(const uint16_t num_users) {
    PacketType = packet_id::ATHENA_ZH_USER_COUNT;
    num_users_ = num_users;
  }

  explicit ATHENA_ZH_USER_COUNT(const uint16_t num_users) {
    emplace(num_users);
  }

  uint32_t num_users() const {
    return num_users_;
  }

  uint16_t PacketType;
private:
  uint16_t num_users_;
};
