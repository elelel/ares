#pragma once

#include <ares/network>
#include <ares/packets>

#include "../macros.h"

namespace ares {
  namespace account {
    struct state;
    struct session;

    namespace mono {
      struct state;
    }
    
    namespace char_server {
      /*! State for sessions that are character servers */
      struct state {
        state(account::state& server_state, session& sess);
        /*! Constructor to create character server state from monostate
          \param mono_state monostate to create character server state from */
        state(const mono::state& mono_state);
        
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
        /*! Characer server's login */
        std::string login;
        /*! Character server's name */
        std::string name;
        /*! Character server's IPv4 address */
        boost::asio::ip::address_v4 ip_v4;
        /*! Character server's IPv4 port */
        uint16_t port;
        uint16_t state_num;
        uint16_t property;

        /*! Number of users online in this character server */
        uint32_t user_count{0};

      private:
        account::state& server_state_;
        session& session_;
      };
      
      ARES_DECLARE_PACKET_HANDLER_TEMPLATE();

      ARES_SIMPLE_PACKET_HANDLER(ATHENA_HA_PING_REQ);
      ARES_SIMPLE_PACKET_HANDLER(ATHENA_HA_ONLINE_AIDS);
      ARES_SIMPLE_PACKET_HANDLER(ATHENA_HA_USER_COUNT);
      ARES_SIMPLE_PACKET_HANDLER(ATHENA_HA_AID_AUTH_REQ);
      ARES_SIMPLE_PACKET_HANDLER(ATHENA_HA_ACCOUNT_DATA_REQ);
      ARES_SIMPLE_PACKET_HANDLER(ATHENA_HA_SET_AID_ONLINE);
      ARES_SIMPLE_PACKET_HANDLER(ATHENA_HA_SET_AID_OFFLINE);
    }
  }
}
