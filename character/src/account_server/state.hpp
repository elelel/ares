#pragma once

#include <ares/network>
#include <ares/packets>

#include "timers.hpp"

namespace ares {
  namespace character {
    struct state;
    struct session;
    
    namespace account_server {
      struct state {
        state(character::state& server_state, session& sess);

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

      public:
        timer::reconnect reconnect_timer;
        timer::send_aids send_aids_timer;
        timer::send_user_count send_user_count_timer;
        timer::ping_request ping_request_timer;
        timer::ping_timeout ping_timeout_timer;
      };
      
      ARES_DECLARE_PACKET_HANDLER_TEMPLATE(character);

      // Simple packet handlers that do not define their own class structure
      ARES_SIMPLE_PACKET_HANDLER(character, ATHENA_AH_LOGIN_RESULT);
      ARES_SIMPLE_PACKET_HANDLER(character, ATHENA_AH_PING_ACK);
      ARES_SIMPLE_PACKET_HANDLER(character, ATHENA_AH_AID_AUTH_RESULT);
      ARES_SIMPLE_PACKET_HANDLER(character, ATHENA_AH_ACCOUNT_DATA_RESULT);
      ARES_SIMPLE_PACKET_HANDLER(character, ATHENA_AH_KICK_AID);
      
      // Packet handlers that store state/structured
    }
  }
}
