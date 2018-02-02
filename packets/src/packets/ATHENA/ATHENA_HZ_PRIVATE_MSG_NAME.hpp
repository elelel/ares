template <typename PacketSet>
struct type<PacketSet, ATHENA_HZ_PRIVATE_MSG_NAME> {
  using packet_name = ATHENA_HZ_PRIVATE_MSG_NAME;
  
  void emplace(const char* name, const size_t name_sz) {
    PacketType = packet_sets::id_of<PacketSet, packet_name>::value;
    dummy_ = 0;
    copy_buf_with_zero_pad(name_, sizeof(name_), name, name_sz);
  }

  void emplace(const std::string& name) {
    emplace(name.c_str(), name.size());
  }
  
  explicit type(const char* name, const size_t name_sz) {
    emplace(name, name_sz);
  }

  explicit type(const std::string& name) {
    emplace(name.c_str(), name.size());
  }

  const char* name() const {
    return name_;
  }

  uint16_t PacketType;
private:
  uint8_t dummy_;
  char name_[24];
};
