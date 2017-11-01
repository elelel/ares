#include "state.hpp"

#include <random>

#include <ares/packets>

#include "../server.hpp"
#include "packet_handlers.hpp"

void ares::account::mono::packet_handler<ares::packet::ATHENA_HA_PING_REQ>::operator()() {
  log()->warn("ATHENA_HA_PING_REQ received from a non-authenticated session, ignoring.");
}
