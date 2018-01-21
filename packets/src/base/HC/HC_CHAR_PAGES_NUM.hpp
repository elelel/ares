struct HC_CHAR_PAGES_NUM {
  inline void emplace(const uint32_t npages,
                      const uint32_t nchars) {
    PacketType = packet_id::HC_CHAR_PAGES_NUM;
    npages_ = npages;
    nchars_ = nchars;
  }

  explicit HC_CHAR_PAGES_NUM(const uint32_t npages,
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

