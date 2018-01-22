struct CH_MAKE_CHAR {
  inline void emplace(const char* name,
                      const size_t name_sz,
                      const uint8_t CharNum,
                      const uint16_t head_palette,
                      const uint16_t head,
                      const uint16_t job,
                      const uint8_t sex) {
    PacketType = packet_id::CH_MAKE_CHAR;
    CharNum_ = CharNum;
    head_palette_ = head_palette;
    head_ = head;
    job_ = job;
    sex_ = sex;

    copy_buf_with_zero_pad(name_, sizeof(name_), name, name_sz);
  }

  inline void emplace(const std::string& name,
                      const uint8_t CharNum,
                      const uint16_t head_palette,
                      const uint16_t head,
                      const uint16_t job,
                      const uint8_t sex) {
    emplace(name.c_str(), name.size(), CharNum, head_palette, head, job, sex);
  }

  explicit CH_MAKE_CHAR(const char* name,
                        const size_t name_sz,
                        const uint8_t CharNum,
                        const uint16_t head_palette,
                        const uint16_t head,
                        const uint16_t job,
                        const uint8_t sex) {
    emplace(name, name_sz, CharNum, head_palette, head, job, sex);
  }

  explicit CH_MAKE_CHAR(const std::string& name,
                      const uint8_t CharNum,
                      const uint16_t head_palette,
                      const uint16_t head,
                      const uint16_t job,
                      const uint8_t sex) {
    emplace(name.c_str(), name.size(), CharNum, head_palette, head, job, sex);
  }

  inline const char* name() const {
    return name_;
  }

  inline uint8_t CharNum() const {
    return CharNum_;
  }

  inline uint16_t head_palette() const {
    return head_palette_;
  }

  inline uint8_t head() const {
    return head_;
  }

  inline uint16_t job() const {
    return job_;
  }

  inline uint8_t sex() const {
    return sex_;
  }

  uint16_t PacketType;
private:
  char name_[24];
  uint8_t CharNum_;
  uint16_t head_palette_;
  uint16_t head_;
  uint16_t job_;
  uint16_t unknown_;
  uint8_t sex_;
};
