#include "pc.hpp"

#include "../session.hpp"

ares::zone::pc::pc(const model::pc_info& info, std::shared_ptr<zone::map> map, std::shared_ptr<zone::session> sess) :
  map(map), info(info), session_(sess) {
}

auto ares::zone::pc::session() const -> const std::weak_ptr<zone::session>& {
  return session_;
}

std::chrono::milliseconds ares::zone::pc::speed() {
  // TODO: Calculate PC's speed
  return std::chrono::milliseconds{150};
}

