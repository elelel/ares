#pragma once

#include <ares/network>
#include <ares/packets>

#include "../config.hpp"

namespace ares {
  namespace account {
    struct server;
    struct session;
    
    namespace client {
      struct state;
    }

    namespace character_server {
      struct state;
    }
    
    namespace mono {
      struct state {
        state(account::server& serv, session& sess);

        friend struct ::ares::account::client::state;
        friend struct ::ares::account::character_server::state;

        /*friend struct ares::account::mono::packe_handler<ares::packet_set, ares::packet::CA_SSO_LOGIN_REQ::login_password>;
          friend struct ares::account::mono::packe_handler<ares::packet_set, ares::packet::CA_SSO_LOGIN_REQ::token_auth>;*/
        
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
        /*! Game client EXE hash (sent before session is handshaken as game client) */
        std::optional<std::array<uint8_t, 16>> client_exe_hash;
        
      private:
        server& server_;
        session& session_;
      };

      ARES_DECLARE_PACKET_HANDLER_TEMPLATE();
      // Simple packet handlers that do not define their own class structure
      ARES_SIMPLE_PACKET_HANDLER(ATHENA_HA_LOGIN_REQ);
      ARES_SIMPLE_PACKET_HANDLER(ATHENA_HA_PING_REQ);
      ARES_SIMPLE_PACKET_HANDLER(CA_EXE_HASHCHECK);
      ARES_SIMPLE_PACKET_HANDLER(CA_SSO_LOGIN_REQ::login_password);
      ARES_SIMPLE_PACKET_HANDLER(CA_SSO_LOGIN_REQ::token_auth);

    }
  }
}
