template <typename PacketSet>
struct type<PacketSet, ARES_HA_ONLINE_ACCOUNTS> {
  using packet_set = PacketSet;
  using packet_name = ARES_HA_ONLINE_ACCOUNTS;
  
  inline void emplace(const size_t count) {
    PacketType = packet_sets::id_of<packet_set, packet_name>::value;
    PacketLength = sizeof(*this) + count * sizeof(uint32_t);
    count_ = count;
  }
  
  inline explicit type(const size_t users_sz) {
    emplace(users_sz);
  }
  
  inline uint32_t count() const {
    return count_;
  }

  inline const model::account_id* account_ids() const {
    return account_ids_;
  }
  
  uint16_t PacketType;
  uint16_t PacketLength;
private:
  uint32_t count_;
  model::account_id account_ids_[];
};
