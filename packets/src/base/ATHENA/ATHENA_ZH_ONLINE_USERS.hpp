struct ATHENA_ZH_ONLINE_USERS {
  struct aid_cid {
    uint32_t aid;
    uint32_t cid;
  };
  
  void emplace(const size_t num_users) {
    PacketType = packet_id::ATHENA_HZ_PRIVATE_MSG_NAME;
    PacketLength = sizeof(ATHENA_ZH_ONLINE_USERS) + sizeof(aid_cid) * num_users;
    num_users_ = num_users;
  }

  explicit ATHENA_ZH_ONLINE_USERS(const size_t num_users) {
    emplace(num_users);
  }

  uint32_t num_users() const {
    return num_users_;
  }

  const aid_cid* users() const {
    return users_;
  }

  uint16_t PacketType;
  uint16_t PacketLength;
private:
  uint16_t num_users_;
  aid_cid users_[];
};
