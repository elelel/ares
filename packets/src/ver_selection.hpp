#pragma once

/*! \file ver_selection.hpp
  \brief Packet set selector, used to interface with make system
 */

namespace ares {
  namespace packet_sets {
    using current = packet_sets::ARES_PACKET_SET;
  }

  namespace packet {
    template <typename PacketName>
    using current = packet::type<packet_sets::ARES_PACKET_SET, PacketName>;
  }
}
