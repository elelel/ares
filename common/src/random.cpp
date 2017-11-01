#include "random.hpp"

#include <limits>

ares::random_int32::random_int32() :
  random_device_(std::random_device()),
  random_engine_(std::default_random_engine(random_device_())),
  dist_(std::uniform_int_distribution<>(std::numeric_limits<int32_t>::min(),
                                            std::numeric_limits<int32_t>::max())) {
}

int32_t ares::random_int32::get() {
  static ares::random_int32 r;
  return r.dist_(r.random_engine_);
}
