#pragma once

#include <cstdlib>
#include <random>

namespace ares {
  struct random_int32 {
    static int32_t get();
  private:
    random_int32();

    std::random_device random_device_;
#if defined(USE_RND_DEF)    
    std::default_random_engine random_engine_;
#else
    std::mt19937 random_engine_;
#endif
    std::uniform_int_distribution<int32_t> dist_;
  };
}
