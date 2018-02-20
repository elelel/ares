template <typename PacketSet>
struct type<PacketSet, CA_EXE_HASHCHECK> {
  using packet_set = PacketSet;
  using packet_name = CA_EXE_HASHCHECK;
  
  inline void emplace(const uint8_t* HashValue,
                      const size_t HashValue_sz) {
    PacketType = packet_sets::id_of<PacketSet, packet_name>::value;
    model::copy_buf_with_zero_pad(HashValue_, sizeof(HashValue_), HashValue, HashValue_sz);
  }

  explicit inline type(const uint8_t* HashValue,
                       const size_t HashValue_sz) {
    emplace(HashValue, HashValue_sz);
  }

  inline const uint8_t* HashValue() const {
    return HashValue_;
  }
  
  uint16_t PacketType;
private:
  uint8_t HashValue_[16];
};
