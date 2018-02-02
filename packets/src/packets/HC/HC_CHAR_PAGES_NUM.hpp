template <typename PacketSet>
struct type<PacketSet, HC_CHAR_PAGES_NUM> {
  using packet_name = HC_CHAR_PAGES_NUM;
  
  inline void emplace(const uint32_t npages,
                      const uint32_t nchars) {
    PacketType = packet_sets::id_of<PacketSet, packet_name>::value;
    npages_ = npages;
    nchars_ = nchars;
  }

  explicit type(const uint32_t npages,
                const uint32_t nchars) {
    emplace(npages, nchars);
  }


  uint32_t npages() const {
    return npages_;
  }

  uint32_t nchars() const {
    return nchars_;
  }
  
  uint16_t PacketType;
  
private:
  uint32_t npages_;
  uint32_t nchars_;

};

