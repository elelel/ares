template <typename PacketSet>
struct type<PacketSet, ZC_ACK_SCHEDULER_CASHITEM> {
  using packet_set = PacketSet;
  using packet_name = ZC_ACK_SCHEDULER_CASHITEM;

  struct item_record {
    uint16_t itemid;
    uint16_t price;
  };
  
  uint16_t length() const {
    return length_;
  }
  
  uint16_t itemCount() const {
    return itemCount_;
  }
  
  uint16_t index() const {
    return index_;
  }

  const item_record* items() const {
    return items_;
  }
  
  uint16_t PacketType;
private:
  uint16_t length_;
  uint16_t itemCount_;
  uint16_t index_;
  item_record items_[];
};
