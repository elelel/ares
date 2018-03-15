template <typename PacketSet>
struct type<PacketSet, ARES_ZH_GAME_RATES> {
  using packet_set = PacketSet;
  using packet_name = ARES_ZH_GAME_RATES;
  
  inline void emplace(const uint32_t base_rate,
               const uint32_t job_rate,
               const uint32_t drop_rate) {
    PacketType = packet_sets::id_of<PacketSet, packet_name>::value;
    base_rate_ = base_rate;
    job_rate_ = job_rate;
    drop_rate_ = drop_rate;
  }

  inline explicit type(const uint32_t base_rate,
                const uint32_t job_rate,
                const uint32_t drop_rate) {
    emplace(base_rate, job_rate, drop_rate);
  }

  inline uint32_t base_rate() const {
    return base_rate_;
  }

  inline uint32_t job_rate() const {
    return job_rate_;
  }

  inline uint32_t drop_rate() const {
    return drop_rate_;
  }

  uint16_t PacketType;
private:
  uint32_t base_rate_;
  uint32_t job_rate_;
  uint32_t drop_rate_;
};
