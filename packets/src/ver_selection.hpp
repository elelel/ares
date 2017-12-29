#pragma once

/*! \file ver_selection.hpp
  \brief Packet version selector, used to interface with make system
 */

namespace ares {
  using packet_id = packet_ids<packet_version::ARES_PACKET_VER>;
  using packet = packets<packet_version::ARES_PACKET_VER>;
}
