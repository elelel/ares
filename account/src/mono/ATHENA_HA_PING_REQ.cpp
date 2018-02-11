#include "state.hpp"

#include "../server.hpp"

void ares::account::mono::packet_handler<ares::packet::current<ares::packet::ATHENA_HA_PING_REQ>>::operator()() {
  log()->warn("ATHENA_HA_PING_REQ received from a non-authenticated session, ignoring.");
}
