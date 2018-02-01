#include "state.hpp"

#include "../state.hpp"

void ares::account::mono::packet_handler<ares::packet<ares::packets::ATHENA_HA_PING_REQ>>::operator()() {
  log()->warn("ATHENA_HA_PING_REQ received from a non-authenticated session, ignoring.");
}
