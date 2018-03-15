template <typename PacketSet>
struct type<PacketSet, ARES_AH_ACCOUNT_AUTH_RESULT> {
  using packet_set = PacketSet;
  using packet_name = ARES_AH_ACCOUNT_AUTH_RESULT;

  void emplace(const uint32_t request_id,
               const model::account_id& account_id,
               const uint8_t result) {
    PacketType = packet_sets::id_of<PacketSet, packet_name>::value;
    request_id_ = request_id;
    account_id_ = account_id;
    result_ = result;
  }

  explicit inline type(const uint32_t request_id,
                       const model::account_id& account_id,
                       const uint8_t result) {
    emplace(request_id, account_id, result);
  }

  inline const model::account_id& account_id() const {
    return account_id_;
  }

  inline int32_t request_id() const {
    return request_id_;
  }

  inline uint8_t result() const {
    return result_;
  }

  uint16_t PacketType;
private:
  uint32_t request_id_;
  model::account_id account_id_;
  uint8_t result_;
};
