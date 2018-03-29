#pragma once

/*! \file size.hpp

    \brief Static packet size calculation structures.
    
    Packet size for a fixed-size packet is equal to the size of it's struct declaration.
    For dynamic-sized packets it equals the value of PacketLength member. Dynamic-sized packets
    are destinguished from fixed-size packets by the presence of this member in the structure.
    The member has to be public, otherwise the detection will assume it's a fixed-size packet.
 */

#include <tuple>

namespace ares {
  namespace packet {
    struct size {
      /*! Calculate how many bytes are still to be received, judging by header/length
        \tparam Packet packet struct type
        \param buf pointer to buffer with packet's contents
        \param sz number of bytes already in the buffer
       */
      template <typename Packet>
      static size_t bytes_needed(const Packet* buf ,
                                 const size_t sz) {
        return bytes_needed_<Packet>(buf, sz, dynamic_size());
      }

      /*! Get the packet size in fifo buffer
        \tparam Packet packet struct type
        \tparam Buffer type of fifo buffer
        \param buf universal reference to the buffer
      */
      template <typename Packet,
                typename Buffer>
      static size_t get(Buffer&& buf) {
        return get_((Packet*)buf.start_ptr(), buf.get(), dynamic_size());
      }

      /*! Get the packet size in flat memory buffer
         \tparam Packet packet struct type
         \param buf plain memory pointer to the Packet
         \param sz number of meaningful bytes in the buffer
      */
      template <typename Packet>
      static size_t get(const Packet* buf, const size_t sz) {
        return get_(buf, sz, dynamic_size());
      }
    
      /*! Check if size declaration for a packet is correct
        \tparam Packet packet struct type
        \param p const reference to the packet struct
      */
      template <typename Packet>
      static size_t validate(const Packet& p) {
        return validate_(p, dynamic_size());
      }

      template <typename Packet>
      static constexpr std::tuple<size_t, size_t, size_t> allocate_size() {
        Packet* p = nullptr;
        return allocate_size_(p, dynamic_size());
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
      static size_t get_(Packet* p, const size_t sz, dynamic_size) {
        const size_t min_for_PacketLength = (uintptr_t)&(p->PacketLength) - (uintptr_t)p + sizeof(decltype(p->PacketLength));
        if (sz < min_for_PacketLength) {
          // PacketLength member has to be reachable, if it's not, return the packet length we have
          return sz;
        }
        return min(sz, p->PacketLength);
      }

      template <typename Packet>
      static size_t get_(Packet*, const size_t sz, fixed_size) {
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

      template <typename Packet,
                typename int_<decltype(Packet::PacketLength)>::type = 0>
      static constexpr std::tuple<size_t, size_t, size_t> allocate_size_(Packet* p, dynamic_size) {
        return std::tuple<size_t, size_t, size_t>
          (sizeof(Packet),
           (sizeof(Packet) > 512) ? sizeof(Packet) + (1024 - (sizeof(Packet) % 1024)) + 1024 : sizeof(Packet),
           (uintptr_t)&p->PacketLength - (uintptr_t)p);
      }

      template <typename Packet>
      static constexpr std::tuple<size_t, size_t, size_t> allocate_size_(Packet*, fixed_size) {
        return std::tuple<size_t, size_t, size_t>
          (sizeof(Packet),
           (sizeof(Packet) > 512) ? sizeof(Packet) + (1024 - (sizeof(Packet) % 1024)) : sizeof(Packet),
           0);
      }
      
      
    };
  }
}
