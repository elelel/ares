#include "../id.hpp"

inline ares::model::detail::id_base::id_base() {
}

inline ares::model::detail::id_base::id_base(const uint32_t& uint32) :
  value_(uint32) {
}

inline ares::model::detail::id_base::id_base(const id_base& other) :
  value_(other.value_) {
}

inline ares::model::detail::id_base::id_base(id_base&& other) :
  value_(std::move(other.value_)) {
}

inline std::string ares::model::detail::id_base::id_base::to_string() const {
  return std::to_string(value_);
}

inline ares::model::AID::AID() :
  id_base() {
}

inline ares::model::AID::AID(const account_id& acc_id) :
  detail::id_base(acc_id.value_ + AID_limits<account_id>::min()) {
}

inline auto ares::model::AID::from_uint32(const uint32_t& uint32) -> AID {
  return AID{uint32};
}

inline bool ares::model::AID::operator==(const AID& other) const {
  return value_ == other.value_;
}

inline bool ares::model::AID::operator<(const AID& other) const {
  return value_ < other.value_;
}

template <>
inline bool ares::model::AID::in_bounds<ares::model::account_id>() const {
  return (value_ > AID_limits<account_id>::min()) && (value_ < AID_limits<account_id>::max());
}

inline auto ares::model::AID::operator=(const AID& other) -> AID& {
  value_ = other.value_;
  return *this;
}


inline ares::model::GID::GID() :
  id_base() {
}

inline ares::model::GID::GID(const character_id& char_id) :
  detail::id_base(char_id.value_ + GID_limits<character_id>::min()) {
}

inline auto ares::model::GID::from_uint32(const uint32_t& uint32) -> GID {
  return GID{uint32};
}

inline bool ares::model::GID::operator==(const GID& other) const {
  return value_ == other.value_;
}

inline bool ares::model::GID::operator<(const GID& other) const {
  return value_ < other.value_;
}

template <>
inline bool ares::model::GID::in_bounds<ares::model::character_id>() const {
  return (value_ > GID_limits<character_id>::min()) && (value_ < GID_limits<character_id>::max());
}

inline auto ares::model::GID::operator=(const GID& other) -> GID& {
  value_ = other.value_;
  return *this;
}


inline ares::model::account_id::account_id() :
  id_base() {
}

inline ares::model::account_id::account_id(const AID& aid) :
  detail::id_base(aid.value_ - AID_limits<account_id>::min()) {
}

inline auto ares::model::account_id::operator=(const account_id& other) -> account_id& {
  value_ = other.value_;
  return *this;
}

inline auto ares::model::account_id::from_uint32(const uint32_t& uint32) -> account_id {
  return account_id{uint32};
}

inline bool ares::model::account_id::operator==(const account_id& other) const {
  return value_ == other.value_;
}

inline bool ares::model::account_id::operator<(const account_id& other) const {
  return value_ < other.value_;
}


inline ares::model::character_id::character_id() :
  id_base() {
}

inline ares::model::character_id::character_id(const GID& gid) :
  detail::id_base(gid.value_ - GID_limits<character_id>::min()) {
}

inline auto ares::model::character_id::operator=(const character_id& other) -> character_id& {
  value_ = other.value_;
  return *this;
}

inline auto ares::model::character_id::from_uint32(const uint32_t& uint32) -> character_id {
  return character_id{uint32};
}

inline bool ares::model::character_id::operator==(const character_id& other) const {
  return value_ == other.value_;
}

inline bool ares::model::character_id::operator<(const character_id& other) const {
  return value_ < other.value_;
}
