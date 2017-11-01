struct CA_EXE_HASHCHECK {
  void emplace(const uint8_t* HashValue,
               const size_t HashValue_sz) {
    PacketType = packet_id::CA_EXE_HASHCHECK;
    copy_buf_with_zero_pad(HashValue_, sizeof(HashValue_), HashValue, HashValue_sz);
  }

  explicit CA_EXE_HASHCHECK(const uint8_t* HashValue,
                            const size_t HashValue_sz) {
    emplace(HashValue, HashValue_sz);
  }

  const uint8_t* HashValue() const {
    return HashValue_;
  }
  
  uint16_t PacketType;
private:
  uint8_t HashValue_[16];
};
