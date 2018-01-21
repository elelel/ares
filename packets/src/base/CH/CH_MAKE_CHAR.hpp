struct CH_MAKE_CHAR {
  inline void emplace(const char* name,
                      const size_t name_sz,
                      uint8_t Str,
                      uint8_t Agi,
                      uint8_t Vit,
                      uint8_t Int,
                      uint8_t Dex,
                      uint8_t Luk,
                      uint8_t CharNum,
                      int16_t headPal,
                      int16_t head) {
    PacketType = packet_id::CH_MAKE_CHAR;
    copy_buf_with_zero_pad(name_, sizeof(name_), name, name_sz);
    Str_ = Str;
    Agi_ = Agi;
    Vit_ = Vit;
    Int_ = Int;
    Dex_ = Dex;
    Luk_ = Luk;
    CharNum_ = CharNum;
    headPal_ = headPal;
    head_ = head;
  }

  inline void emplace(const std::string& name,
                      uint8_t Str,
                      uint8_t Agi,
                      uint8_t Vit,
                      uint8_t Int,
                      uint8_t Dex,
                      uint8_t Luk,
                      uint8_t CharNum,
                      int16_t headPal,
                      int16_t head) {
    emplace(name.c_str(), name.size(), Str, Agi, Vit, Int, Dex, Luk, CharNum, headPal, head);
  }

  explicit CH_MAKE_CHAR(const char* name,
                        const size_t name_sz,
                        uint8_t Str,
                        uint8_t Agi,
                        uint8_t Vit,
                        uint8_t Int,
                        uint8_t Dex,
                        uint8_t Luk,
                        uint8_t CharNum,
                        int16_t headPal,
                        int16_t head) {
    emplace(name, name_sz, Str, Agi, Vit, Int, Dex, Luk, CharNum, headPal, head);
  }

  explicit CH_MAKE_CHAR(const std::string& name,
                        uint8_t Str,
                        uint8_t Agi,
                        uint8_t Vit,
                        uint8_t Int,
                        uint8_t Dex,
                        uint8_t Luk,
                        uint8_t CharNum,
                        int16_t headPal,
                        int16_t head) {
    emplace(name, Str, Agi, Vit, Int, Dex, Luk, CharNum, headPal, head);
  }

  inline const char* name() const {
    return name_;
  }

  inline uint8_t Str() const {
    return Str_;
  }

  inline uint8_t Agi() const {
    return Agi_;
  }

  inline uint8_t Vit() const {
    return Vit_;
  }

  inline uint8_t Int() const {
    return Int_;
  }

  inline uint8_t Dex() const {
    return Dex_;
  }

  inline uint8_t Luk() const {
    return Luk_;
  }

  inline uint8_t charNum() const {
    return CharNum_;
  }

  inline int16_t headPal() const {
    return headPal_;
  }

  inline uint8_t head() const {
    return head_;
  }
  
  uint16_t PacketType;
  
private:
  char name_[24];
  uint8_t Str_;
  uint8_t Agi_;
  uint8_t Vit_;
  uint8_t Int_;
  uint8_t Dex_;
  uint8_t Luk_;
  uint8_t CharNum_;
  int16_t headPal_;
  int16_t head_;
};