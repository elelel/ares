#pragma once

#include <ares/network>
#include <ares/packets>

namespace ares {
  namespace character {
    struct state;
    struct session;

    namespace client {
      struct state;
    }

    namespace zone_server {
      struct state;
    }
    
    namespace mono {
      struct state {
        friend struct client::state;
        friend struct zone_server::state;
        
        state(character::state& state, session& sess);        
        // Interface with character::session
        void on_open();
        void before_close();
        void on_connection_reset();
        void on_eof();
        void on_socket_error();
        void on_operation_aborted();
        size_t dispatch(const uint16_t PacketType);

        void defuse_asio();
      private:
        character::state& server_state_;
        session& session_;
      };
      
      ARES_DECLARE_PACKET_HANDLER_TEMPLATE(character);
      
      // Simple packet handlers that do not define their own class structure
      ARES_SIMPLE_PACKET_HANDLER(character, ATHENA_ZH_LOGIN_REQ);
      
      // Packet handlers that store state/structured
      #include "CH_ENTER.hpp"
    }
  }
}
