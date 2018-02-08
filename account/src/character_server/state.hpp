#pragma once

#include <ares/network>
#include <ares/packets>

#include "../config.hpp"

namespace ares {
  namespace account {
    struct server;
    struct session;

    namespace mono {
      struct state;
    }
    
    namespace character_server {
      struct state {
        state(account::server& serv, session& sess);
        /*! Constructor to create character server state from monostate
          \param mono_state monostate to create character server state from */
        state(const mono::state& mono_state);
        
        void on_connect();
        void on_connection_reset();
        void on_operation_aborted();
        void on_eof();
        void on_socket_error();
        void on_packet_processed();

        size_t dispatch_packet(const uint16_t packet_id);
        
        std::shared_ptr<spdlog::logger> log() const;
        const config& conf() const;

        // Data
        /*! Characer server's login */
        std::string login;
        /*! Character server's name */
        std::string name;
        /*! Character server's IPv4 address */
        asio::ip::address_v4 ip_v4;
        /*! Character server's IPv4 port */
        uint16_t port;
        uint16_t state_num;
        uint16_t property;

        /*! Number of users online in this character server */
        uint32_t user_count{0};

      private:
        server& server_;
        session& session_;


      };

      ARES_DECLARE_PACKET_HANDLER_TEMPLATE();
      // Simple packet handlers that do not define their own class structure
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
