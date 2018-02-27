template <typename PacketSet>
struct type<PacketSet, CH_MAKE_CHAR::with_stats> {
  using packet_set = PacketSet;
  using packet_name = CH_MAKE_CHAR::with_stats;
  
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
    PacketType = PacketSet::template id_of<CH_MAKE_CHAR::with_stats>::value;
    name_ = name;
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

  explicit type(const std::string& name,
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

template <typename PacketSet>
struct type<PacketSet, CH_MAKE_CHAR::no_stats> {
  using packet_name = CH_MAKE_CHAR::no_stats;
  
  inline void emplace(const std::string& name,
                      const uint8_t CharNum,
                      const uint16_t head_palette,
                      const uint16_t head,
                      const model::pc_job job,
                      const uint8_t sex) {
    PacketType = PacketSet::template id_of<CH_MAKE_CHAR::no_stats>::value;
    name_ = name;
    CharNum_ = CharNum;
    head_palette_ = head_palette;
    head_ = head;
    job_ = job;
    sex_ = sex;
  }

  explicit inline type(const std::string& name,
                       const uint8_t CharNum,
                       const uint16_t head_palette,
                       const uint16_t head,
                       const model::pc_job job,
                       const uint8_t sex) {
    emplace(name, CharNum, head_palette, head, job, sex);
  }

  inline const model::pc_name_string& name() const {
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

  inline model::pc_job job() const {
    return job_;
  }

  inline uint8_t sex() const {
    return sex_;
  }

  uint16_t PacketType;
private:
  model::pc_name_string name_;
  uint8_t CharNum_;
  uint16_t head_palette_;
  uint16_t head_;
  model::pc_job job_;
  uint16_t unknown_;
  uint8_t sex_;
};

