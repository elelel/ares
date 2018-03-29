#include "memory.hpp"

auto ares::memory::thread_local_pool() -> pmr::unsynchronized_pool_resource& {
  pmr::pool_options opts;
  opts.max_blocks_per_chunk = max_blocks_per_chunk;
  opts.largest_required_pool_block = largest_required_pool_block;
  static thread_local pmr::unsynchronized_pool_resource p(opts);
  return p;
}

auto ares::memory::static_pool() -> pmr::synchronized_pool_resource& {
  pmr::pool_options opts;
  opts.max_blocks_per_chunk = max_blocks_per_chunk;
  opts.largest_required_pool_block = largest_required_pool_block;
  static pmr::synchronized_pool_resource p(opts);
  return p;
}

ares::memory::packet_alloc::packet_alloc(const size_t expected_sz, const size_t buf_sz, const size_t PacketLength_offset) :
  expected_sz(expected_sz),
  buf_sz(buf_sz),
  PacketLength_offset(PacketLength_offset),
  buf(std::shared_ptr<std::byte[]>(reinterpret_cast<std::byte*>(memory::static_pool().allocate(buf_sz * sizeof(std::byte), alignof(std::byte))),
                                   [buf_sz] (std::byte* p) {
                                     memory::static_pool().deallocate(p, buf_sz * sizeof(std::byte), alignof(std::byte));
                                   })) {
}
                                   
ares::memory::packet_alloc::packet_alloc(const std::tuple<size_t, size_t, size_t> sizes) :
  packet_alloc(std::get<0>(sizes), std::get<1>(sizes), std::get<2>(sizes)) {
}

ares::memory::packet_alloc::packet_alloc(const packet_alloc& other) :
  expected_sz(other.expected_sz),
  buf_sz(other.buf_sz),
  PacketLength_offset(other.PacketLength_offset),
  buf(other.buf) {
}

ares::memory::packet_alloc::packet_alloc(packet_alloc&& other) :
  expected_sz(other.expected_sz),
  buf_sz(other.buf_sz),
  PacketLength_offset(other.PacketLength_offset),
  buf(std::move(other.buf)) {
}

auto ares::memory::packet_alloc::operator=(packet_alloc&& other) -> ares::memory::packet_alloc& {
  expected_sz = std::move(other.expected_sz);
  buf_sz = std::move(other.buf_sz);
  PacketLength_offset = std::move(other.PacketLength_offset);
  buf = std::move(other.buf);
  return *this;
}
