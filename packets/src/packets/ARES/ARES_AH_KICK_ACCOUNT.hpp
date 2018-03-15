template <typename PacketSet>
struct type<PacketSet, ARES_AH_KICK_ACCOUNT> {
  using packet_set = PacketSet;
  using packet_name = ARES_AH_KICK_ACCOUNT;
  
  inline void emplace(const model::account_id& account_id) {
    PacketType = packet_sets::id_of<packet_set, packet_name>::value;
    account_id_ = account_id;
  }
  
  inline explicit type(const model::account_id& account_id) {
    emplace(account_id);
  }

  inline const model::account_id& account_id() const {
    return account_id_;
  }
  
  uint16_t PacketType;
private:
  model::account_id account_id_;
};
