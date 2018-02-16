#pragma once

#include <functional>

#include <foonathan/memory/memory_pool.hpp>

namespace ares {
  namespace packet {
    struct alloc_info {
      size_t expected_packet_sz;
      void* buf;
      size_t buf_sz;
      std::function<void(void*)> deallocator;
      size_t PacketLength_offset;
    };

    template <size_t Size>
    struct memory_pool {
      inline static foonathan::memory::memory_pool<foonathan::memory::node_pool>& instance() {
        // TODO: Should I add align difference to Size or foonathan does that for me?
        static foonathan::memory::memory_pool<foonathan::memory::node_pool> pool(Size, Size*32);
        return pool;
      }
      
      memory_pool() = delete;
    };

    template <size_t Size>
    struct pool_allocator {
      using pool_type = memory_pool<Size>;

      inline static std::tuple<void*, size_t> allocate() {
        return std::make_tuple(pool_type::instance().allocate_node(), Size);
      }

      inline static void deallocate(void* p) {
        pool_type::instance().deallocate_node(p);
      }
    };

    struct send_buf_allocator : pool_allocator<64 * 1024> {
    };


    
    template <typename Packet>
    std::shared_ptr<Packet> make_shared_packet(void* buf, std::function<void(void*)> deallocator) {
      return std::shared_ptr<Packet>((Packet*)buf, [deallocator] (Packet* p) { deallocator((void*)p); });
    }
  }
}
