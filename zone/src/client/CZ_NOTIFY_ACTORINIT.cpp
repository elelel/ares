#include "state.hpp"

#include "../server.hpp"

void ares::zone::client::packet_handler<ares::packet::current<ares::packet::CZ_NOTIFY_ACTORINIT>>::operator()() {
  SPDLOG_TRACE(log(), "handle_packet CZ_NOTIFY_ACTORINIT: begin");

  /* TODO: Actor init sequence (sending to client order)

     Change look (weapon)
     Cloth
     Body
     Inventory items
     Cart
     Weight/max weight
     Guild member info
     Spawn pc
     Party map, hp
     Battleground hp
     Map property
     Nearby objects
     Pet
     Homunculus
     Mercenary
     Elemental
     
        New connections
     Skills
     Hotkeys
     Status: base exp, next base exp, job exp, next job exp, skill point, ZC_STATUS,
             option status (falcon, riding, wurgrider, all_riding)
     Skill knowledge
     Pet emotion
     Night
     
        Map change/waps
     Status: str, agi, vit, int, dex, luk
     ...
             
   */

  SPDLOG_TRACE(log(), "handle_packet CZ_NOTIFY_ACTORINIT: end");
}
