/*! \file packets.hpp
\brief Packetsets description (packet database)

This file is the top file to add packetset names and to include the packetsets hierarchy.
Naming convention is pets_xxxxxxxx where xxxxxxxx is portable executable's timestamp as
hex uint32 of ragexe 
*/

#pragma once

#define ARES_PACKETSET_DECLARE(PACKETSET, BASE)                         \
  struct PACKETSET : BASE {};                                           \
                                                                        \
  template <uint16_t PacketId>                                          \
  struct name_of<PACKETSET, PacketId> : name_of<BASE, PacketId> {       \
    using type = typename name_of<BASE, PacketId>::type;                \
  };                                                                    \
                                                                        \
  template <typename T>                                                 \
  struct id_of<PACKETSET, T> : id_of<BASE, T> {                         \
  };                                                                    \


#define ARES_PACKETSET_ID_NAME(PACKETSET, ID, NAME)                     \
  template <> struct name_of<PACKETSET, ID> { using type = packet::NAME; }; \
  template <> struct id_of<PACKETSET, packet::NAME> { constexpr static const uint16_t value = ID; }; \

namespace ares {
  namespace packet_sets {
    struct base {};
    
    template <typename PacketSet, uint16_t PacketId>
    struct name_of {};
      
    template <typename PacketSet, typename T>
    struct id_of {};
  }
}

#include "server.hpp"  // Intraserver packets
#include "client_stable.hpp"  // Client packets that never changed
#include "pets_5a3cb64a.hpp"  // 2017-12-27aRagexeRE.exe
#include "pets_5a4c7c5c.hpp"  // 2018-01-03bRagexeRE.exe


