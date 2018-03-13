template <typename PacketSet>
struct type<PacketSet, ZC_NOTIFY_TIME> {
  using packet_set = PacketSet;
  using packet_name = ZC_NOTIFY_TIME;
  
  inline void emplace() {
    PacketType = packet_sets::id_of<packet_set, packet_name>::value;
    auto duration = std::chrono::steady_clock::now().time_since_epoch();
    time_ = std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();
  }

  inline explicit type() {
    emplace();
  }

  uint16_t PacketType;
private:
  uint32_t time_;
};
