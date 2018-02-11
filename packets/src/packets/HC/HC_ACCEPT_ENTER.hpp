template <typename PacketSet>
struct type<PacketSet, HC_ACCEPT_ENTER> {
  using packet_set = PacketSet;
  using packet_name = HC_ACCEPT_ENTER;
  
  inline void emplace(const uint8_t normal_slots,
               const uint8_t premium_slots,
               const uint8_t billing_slots,
               const uint8_t creatable_slots,
               const uint8_t valid_slots,
               const size_t characters_count) {
    PacketType = packet_sets::id_of<PacketSet, packet_name>::value;
    PacketLength = sizeof(*this) + characters_count * sizeof(CHARACTER_INFO);
    normal_slots_ = normal_slots;
    premium_slots_ = premium_slots;
    billing_slots_ = billing_slots;
    creatable_slots_ = creatable_slots;
    valid_slots_ = valid_slots;
    billing_dummy1_ = 0;
    code_ = 0;
    time1_ = 0;
    time2_ = 0;
    memset(billing_dummy2_, 0, sizeof(billing_dummy2_));
  }

  explicit type(const uint8_t normal_slots,
                const uint8_t premium_slots,
                const uint8_t billing_slots,
                const uint8_t creatable_slots,
                const uint8_t valid_slots,
                const size_t characters_count) {
    emplace(normal_slots, premium_slots, billing_slots, creatable_slots, valid_slots, characters_count);
  }

  struct CHARACTER_INFO {
    // TODO: define to support old clients
  };

  inline uint8_t normal_slots() const {
    return normal_slots_;
  }

  inline uint8_t premium_slots() const {
    return premium_slots_;
  }

  inline uint8_t billing_slots() const {
    return billing_slots_;
  }

  inline uint8_t creatable_slots() const {
    return creatable_slots_;
  }

  const CHARACTER_INFO* characters() const {
    return characters_;
  }

  uint16_t PacketType;
  uint16_t PacketLength;

private:
  /*! number of standard slots */
  uint8_t normal_slots_;
  /*! number of additional premium slots with "Premium service" caption in client*/
  uint8_t premium_slots_;
  /*! number of additional billing service with "Billing service" caption in client */
  uint8_t billing_slots_;
  /*! adds "N/A" caption to any slot past this number */
  uint8_t creatable_slots_;
  /*! adds "N/A" caption to any slot past this number and blocks char creation (but not selection) clientside */
  uint8_t valid_slots_;
  int8_t billing_dummy1_;
  uint32_t code_;
  uint32_t time1_;
  uint32_t time2_;
  char billing_dummy2_[7];
  /*! character descriptors, always empty in modern clients */
  CHARACTER_INFO characters_[];
};
