#pragma once

#include <foonathan/memory/container.hpp>
#include <foonathan/memory/memory_pool.hpp>

namespace ares {
  namespace model {
    using mempool_type = foonathan::memory::memory_pool_collection<foonathan::memory::node_pool,
                                                                   foonathan::memory::log2_buckets>;
    
  }
}
