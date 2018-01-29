#pragma once

#include <ares/network>
#include <ares/packets>

#include "../macros.h"
#include "timers.hpp"

namespace ares {
  namespace zone {
    struct state;
    struct session;
    
    namespace character_server {
      struct state {
        state(zone::state& server_state, session& sess);
        
        // Interface with zone::session
        void on_open();
        void before_close();
        void on_connection_reset();
        void on_eof();
        void on_socket_error();
        void on_operation_aborted();
        size_t dispatch(const uint16_t PacketType);

        void defuse_asio();

      private:
        zone::state& server_state_;
        session& session_;

      public:
        timer::reconnect reconnect_timer;
        timer::ping_request ping_request_timer;
        timer::ping_timeout ping_timeout_timer;

        // Data
        std::string private_msg_name{"Character server"};        
      };

      ARES_DECLARE_PACKET_HANDLER_TEMPLATE();
      
      // Simple packet handlers that do not define their own class structure
      ARES_SIMPLE_PACKET_HANDLER(ATHENA_HZ_LOGIN_RESULT);
      ARES_SIMPLE_PACKET_HANDLER(ATHENA_HZ_PRIVATE_MSG_NAME);
      ARES_SIMPLE_PACKET_HANDLER(ATHENA_HZ_PING_ACK);
      // Packet handlers that store state/structured
    }
  }
}

