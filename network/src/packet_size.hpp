#pragma once

namespace ares {
  namespace network {
    struct packet_size_helper {
      // How many bytes are needed to receive, judging by header/length
      template <typename Packet>
      static size_t bytes_needed(const Packet* buf, const size_t sz) {
        return bytes_needed_<Packet>(buf, sz, dynamic_size());
      }

      // How many bytes should be in fifo_buffer, judging by header/length
      template <typename Packet,
                typename Buffer>
      static size_t size(Buffer&& buf) {
        return size_<>((Packet*)buf.start_ptr(), buf.size(), dynamic_size());
      }

      // How many bytes should be in void* buffer, judging by header/length
      template <typename Packet>
      static size_t size(const Packet* buf, const size_t sz) {
        return size_(buf, sz, dynamic_size());
      }
    
      // Check if size declaration for packet reference is correct
      template <typename Packet>
      static size_t validate(const Packet& p) {
        return validate_(p, dynamic_size());
      }

    private:
      template <typename> struct int_ { typedef int type; };

      struct fixed_size {};
      struct dynamic_size : fixed_size {};

      template <typename Packet,
                typename int_<decltype(Packet::PacketLength)>::type = 0>
      static size_t bytes_needed_(const Packet* p, const size_t sz, dynamic_size) {
        if (sz < 2) return 2 - sz;
        // If we don't have contents of PacketLength field, require the whole header
        const size_t min_for_PacketLength = (uintptr_t)&(p->PacketLength) - (uintptr_t)p + sizeof(decltype(p->PacketLength));
        if (sz < min_for_PacketLength) return sizeof(Packet) - sz;
        if (sz < p->PacketLength) return p->PacketLength - sz;
        return 0;
      }

      template <typename Packet>
      static size_t bytes_needed_(const Packet*, const size_t sz, fixed_size) {
        if (sz < 2) return 2 - sz;
        if (sz < sizeof(Packet)) return sizeof(Packet) - sz;
        return 0;
      }

      template <typename Packet,
                typename int_<decltype(Packet::PacketLength)>::type = 0>
      static size_t size_(Packet* p, const size_t sz, dynamic_size) {
        const size_t min_for_PacketLength = (uintptr_t)&(p->PacketLength) - (uintptr_t)p + sizeof(decltype(p->PacketLength));
        if (sz < min_for_PacketLength) {
          // PacketLength member has to be reachable, if it's not, return the packet length we have
          return sz;
        }
        return min(sz, p->PacketLength);
      }

      template <typename Packet>
      static size_t size_(Packet*, const size_t sz, fixed_size) {
        if (sz >= sizeof(Packet)) {
          return sizeof(Packet);
        }
        return 0;
      }

      static size_t min(const size_t x, const size_t y) {
        if (x < y) return x;
        return y;
      }
    
      template <typename... Args>
      static size_t min(const size_t x, const size_t y, Args&&... args) {
        if (x < y) return min(x, std::forward<Args>(args)...);
        return min(y, std::forward<Args>(args)...);
      }

      template <typename Packet,
                typename int_<decltype(Packet::PacketLength)>::type = 0>
      static bool validate_(const Packet& p, dynamic_size) {
        return (p.PacketLength >= sizeof(Packet));
      }

      template <typename Packet>
      static bool validate_(const Packet&, fixed_size) {
        return true;
      }
    };
  }
}
