struct ATHENA_HZ_PRIVATE_MSG_NAME {
  void emplace(const char* name, const size_t name_sz) {
    PacketType = packet_id::ATHENA_HZ_PRIVATE_MSG_NAME;
    dummy_ = 0;
    copy_buf_with_zero_pad(name_, sizeof(name_), name, name_sz);
  }

  void emplace(const std::string& name) {
    emplace(name.c_str(), name.size());
  }
  
  explicit ATHENA_HZ_PRIVATE_MSG_NAME(const char* name, const size_t name_sz) {
    emplace(name, name_sz);
  }

  explicit ATHENA_HZ_PRIVATE_MSG_NAME(const std::string& name) {
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
