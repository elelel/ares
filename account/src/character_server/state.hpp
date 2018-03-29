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
        void defuse_asio();

        std::tuple<size_t, size_t, size_t> packet_sizes(const uint16_t packet_id);
        void dispatch_packet(std::shared_ptr<std::byte[]> buf);
        
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
      
      ARES_PACKET_HANDLER(ARES_HA_PING_REQ);
      ARES_PACKET_HANDLER(ARES_HA_ONLINE_ACCOUNTS);
      ARES_PACKET_HANDLER(ARES_HA_USER_COUNT);
      ARES_PACKET_HANDLER(ARES_HA_ACCOUNT_AUTH_REQ);

    }
  }
}
