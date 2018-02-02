template <typename PacketSet>
struct type<PacketSet, ATHENA_ZH_USER_COUNT> {
  using packet_name = ATHENA_ZH_USER_COUNT;
  
  void emplace(const uint16_t num_users) {
    PacketType = packet_sets::id_of<PacketSet, packet_name>::value;
    num_users_ = num_users;
  }

  explicit type(const uint16_t num_users) {
    emplace(num_users);
  }

  uint32_t num_users() const {
    return num_users_;
  }

  uint16_t PacketType;
private:
  uint16_t num_users_;
};
