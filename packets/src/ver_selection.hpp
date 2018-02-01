#pragma once

/*! \file ver_selection.hpp
  \brief Packet set selector, used to interface with make system
 */

namespace ares {
  using packet_set = packet_sets::ARES_PACKET_SET;
  template <typename Name>
  using packet = packets::type<packet_set, Name>;
}
