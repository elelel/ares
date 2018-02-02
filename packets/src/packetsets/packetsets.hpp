/*! \file packets.hpp
\brief Packetsets description (packet database)

This file is the top file to add packetset names and to include the packetsets hierarchy.
Naming convention is pets_xxxxxx where xxxxxx is portable executable's timestamp as
hex uint32 of ragexe this packetset is derived from
*/

#pragma once

#define ARES_PACKETSET_ID_TYPE(PACKETSET, ID, TYPE)    \
  template <> struct PACKETSET::name_of<ID> { using type = packets::type<PACKETSET, packets::TYPE>; }; \
  template <> struct PACKETSET::id_of<packets::type<PACKETSET, packets::TYPE>> { constexpr static const uint16_t value = ID; }; \

namespace ares {
  namespace packet_sets {
    struct base {
      template <uint16_t PacketId> struct name_of {
      };
      
      template <typename T> struct id_of {
      };
    };
  }
}

#include "server.hpp"  // Intraserver packets
#include "pets_5a3cb64a.hpp"   // 2017-12-27aRagexeRE.exe
#include "pets_5a4c7c5c.hpp"   // 2018-01-03bRagexeRE.exe


