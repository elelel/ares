#pragma once

#include <ares/network>
#include <ares/packets>

#include "../macros.h"

namespace ares {
  namespace account {
    struct state;
    struct session;
    
    namespace client {
      struct state;
    }

    namespace char_server {
      struct state;
    }

    namespace mono {
      /*! State for sessions that are monostate (not initialized yet) */
      struct state {
        friend struct client::state;
        friend struct char_server::state;
        
        state(account::state& server_state, session& sess);
        // Interface with account::session
        void defuse_asio();
        void on_open();
        void before_close();
        void on_connection_reset();
        void on_eof();
        void on_socket_error();
        void on_operation_aborted();
        size_t dispatch(const uint16_t PacketType);

        // Data
        /*! Game client EXE hash (sent before session is handshaken as game client) */
        std::optional<std::array<uint8_t, 16>> client_exe_hash;

      private:
        account::state& server_state_;
        session& session_;
      };

      ARES_DECLARE_PACKET_HANDLER_TEMPLATE();

      // Simple packet handlers that do not define their own class structure
      ARES_SIMPLE_PACKET_HANDLER(ATHENA_HA_LOGIN_REQ);
      ARES_SIMPLE_PACKET_HANDLER(ATHENA_HA_PING_REQ);
      ARES_SIMPLE_PACKET_HANDLER(CA_EXE_HASHCHECK);
      
      // Packet handlers that store state/structured
      #include "CA_SSO_LOGIN_REQ_0x64.hpp"
      #include "CA_SSO_LOGIN_REQ.hpp"
      
    }
  }
}
