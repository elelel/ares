template <typename PacketSet>
struct type<PacketSet, ARES_HZ_PRIVATE_MSG_NAME> {
  using packet_set = PacketSet;
  using packet_name = ARES_HZ_PRIVATE_MSG_NAME;
  
  inline void emplace(const std::string& name) {
    PacketType = packet_sets::id_of<PacketSet, packet_name>::value;
    name_ = name;
  }

  explicit inline type(const std::string& name) {
    emplace(name);
  }

  inline const model::fixed_string<24>& name() const {
    return name_;
  }

  uint16_t PacketType;
private:
  model::fixed_string<24> name_;
};
