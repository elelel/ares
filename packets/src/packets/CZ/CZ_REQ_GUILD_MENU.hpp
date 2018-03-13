template <typename PacketSet>
struct type<PacketSet, CZ_REQ_GUILD_MENU> {
  using packet_set = PacketSet;
  using packet_name = CZ_REQ_GUILD_MENU;

  enum class guild_info_type : uint32_t {
    basic_info = 0,
    member_manager,
    positions,
    skills,
    expulsion_list,
    GM_ALLGUILDLIST,
    notice
  };
  
  inline void emplace(const guild_info_type type) {
    PacketType = packet_sets::id_of<packet_set, packet_name>::value;
  }

  inline explicit type(const guild_info_type type) {
    emplace(type);
  }

  guild_info_type info_type() const {
    return type_;
  }
  
  uint16_t PacketType;
private:
  guild_info_type type_;
};
