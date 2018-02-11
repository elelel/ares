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

        friend struct client::state;
        friend struct character_server::state;

        void on_connect();
        void on_connection_reset();
        void on_operation_aborted();
        void on_eof();
        void on_socket_error();
        void on_packet_processed();
        void defuse_asio();

        packet::alloc_info allocate(const uint16_t packet_id);
        void dispatch_packet(const uint16_t packet_id, void* buf, std::function<void(void*)> deallocator);
        
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

      ARES_PACKET_HANDLER(ATHENA_HA_LOGIN_REQ);
      ARES_PACKET_HANDLER(ATHENA_HA_PING_REQ);
      ARES_PACKET_HANDLER(CA_EXE_HASHCHECK);
      ARES_PACKET_HANDLER(CA_SSO_LOGIN_REQ::login_password);
      ARES_PACKET_HANDLER(CA_SSO_LOGIN_REQ::token_auth);

    }
  }
}
