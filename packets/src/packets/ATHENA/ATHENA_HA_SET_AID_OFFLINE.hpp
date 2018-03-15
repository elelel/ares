template <typename PacketSet>
struct type<PacketSet, ATHENA_HA_SET_AID_OFFLINE> {
  using packet_set = PacketSet;
  using packet_name = ATHENA_HA_SET_AID_OFFLINE;
  
  void emplace(const uint32_t aid) {
    PacketType = packet_sets::id_of<PacketSet, packet_name>::value;
    aid_ = aid;
  }
  
  explicit type(const uint32_t aid) {
    emplace(aid);
  }
  
  uint32_t aid() const {
    return aid_;
  }
  
  uint16_t PacketType;
private:
  model::account_id account_id_;
};
