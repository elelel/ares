#pragma once

#include <ares/network>
#include <ares/packets>

namespace ares {
  namespace zone {
    struct state;
    struct session;
    
    namespace client {
      struct state;
    }

    namespace character_server {
      struct state;
    }
    
    namespace mono {
      struct state {
        friend struct client::state;
        friend struct character_server::state;
        
        state(zone::state& zone_state, session& sess);        
        // Interface with zone::session
        void defuse_asio();
        void on_open();
        void before_close();
        void on_connection_reset();
        void on_eof();
        void on_socket_error();
        void on_operation_aborted();
        size_t dispatch(const uint16_t PacketType);

      private:
        zone::state& server_state_;
        session& session_;
      };

      ARES_DECLARE_PACKET_HANDLER_TEMPLATE(zone);
      
    }
  }
}
