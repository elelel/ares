template <typename PacketSet>
struct type<PacketSet, ATHENA_HA_ACCOUNT_DATA_REQ> {
  using packet_set = PacketSet;
  using packet_name = ATHENA_HA_ACCOUNT_DATA_REQ;
  
  void emplace(const uint32_t aid) {
    PacketType = packet_sets::id_of<PacketSet, packet_name>::value;
    aid_ = aid;
  }
  
  explicit type(const uint32_t aid) {
    emplace(aid);
  }

  inline uint32_t aid() const {
    return aid_;
  }

  uint16_t PacketType;
private:
  model::account_id account_id_;
};
