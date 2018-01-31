#pragma once

#include <list>
#include <unordered_set>

#include <ares/network>
#include <ares/packets>

#include "../macros.h"
#include "../mono/state.hpp"

namespace ares {
  namespace character {
    
    namespace zone_server {
      struct state {
        state(character::state& server_state, session& sess);        
        state(const mono::state& mono_state);

        // Interface with character::session
        void on_open();
        void before_close();
        void on_connection_reset();
        void on_eof();
        void on_socket_error();
        void on_operation_aborted();
        size_t dispatch(const uint16_t PacketType);
        void defuse_asio();

        // Data
        std::string login;
        std::list<std::string> maps_to_send;
        std::unordered_set<std::string> map_names;
        
        asio::ip::address_v4 ip_v4;
        uint16_t port;

        std::map<uint32_t, uint32_t> online_aid_to_cid;
        std::map<uint32_t, uint32_t> online_cid_to_aid;
        uint32_t base_rate{0};
        uint32_t job_rate{0};
        uint32_t drop_rate{0};


      private:
        character::state& server_state_;
        session& session_;

      };
      ARES_DECLARE_PACKET_HANDLER_TEMPLATE();

      // Simple packet handlers that do not define their own class structure
      ARES_SIMPLE_PACKET_HANDLER(ATHENA_ZH_ONLINE_USERS);
      ARES_SIMPLE_PACKET_HANDLER(ATHENA_ZH_USER_COUNT);
      ARES_SIMPLE_PACKET_HANDLER(ATHENA_ZH_GAME_RATES);
      ARES_SIMPLE_PACKET_HANDLER(ATHENA_ZH_PING_REQ);
      ARES_SIMPLE_PACKET_HANDLER(ARES_ZH_MAP_NAMES_REQ);
      
      // Packet handlers that store state/structured
    }
  }
}
