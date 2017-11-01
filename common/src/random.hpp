#pragma once

#include <cstdlib>
#include <random>

namespace ares {
  struct random_int32 {
    static int32_t get();
  private:
    random_int32();

    std::random_device random_device_;
    std::default_random_engine random_engine_;
    std::uniform_int_distribution<int32_t> dist_;
  };
}
