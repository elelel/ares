#pragma once

#if !defined(ARES_NO_STD_PMR)
#include "memory_std.hpp"
#else
#include "memory_boost.hpp"
#endif

#include <memory>

namespace ares {
  struct memory {
    memory() = delete;
    
    static constexpr size_t max_blocks_per_chunk = 128;
    /*!
      Maximum pool block size, if exceeded, allocation will fallback to upstream allocator
     */
    static constexpr size_t largest_required_pool_block = 1024 * 1024;
    static pmr::unsynchronized_pool_resource& thread_local_pool();
    static pmr::synchronized_pool_resource& static_pool();

    struct packet_alloc {
      packet_alloc(const size_t expected_sz, const size_t buf_sz, const size_t PacketLength_offset);

      packet_alloc(const std::tuple<size_t, size_t, size_t> sizes);

      packet_alloc(const packet_alloc& other);

      packet_alloc(packet_alloc&& other);

      packet_alloc& operator=(packet_alloc&& other);
      
      size_t expected_sz;
      size_t buf_sz;
      size_t PacketLength_offset;
      std::shared_ptr<std::byte[]> buf;
    };
    
  };
}
