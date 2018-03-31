#include "map.hpp"

ares::zone::map::map(const uint32_t id, const model::map_info& info) :
  id_(id),
  info_(info) {
}

bool ares::zone::map::is_walkable(const uint16_t x, const uint16_t y) const {
  return (x < info_.x_size()) && (y < info_.y_size()) &&
    info_.static_flags(x, y).data() & model::map_cell_flags::walkable;
}

