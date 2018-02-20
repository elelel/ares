#pragma once

#include <ares/network>
#include <ares/packets>

#include "../config.hpp"

namespace ares {
  namespace zone {
    struct server;
    struct session;
    
    namespace character_server {
      struct state {
        state(zone::server& serv, session& sess);

        void on_connect();
        void on_connection_reset();
        void on_operation_aborted();
        void on_eof();
        void on_socket_error();
        void defuse_asio();

        packet::alloc_info allocate(const uint16_t packet_id);
        void dispatch_packet(void* buf, std::function<void(void*)> deallocator);

        std::shared_ptr<spdlog::logger> log() const;
        const config& conf() const;
        
        void reset_ping_character_server_timer();
      private:
        server& server_;
        session& session_;

      public:
        std::shared_ptr<asio::steady_timer> ping_character_server_timer_;

      };

      ARES_DECLARE_PACKET_HANDLER_TEMPLATE();
      
      ARES_PACKET_HANDLER(ATHENA_HZ_LOGIN_RESULT);
      ARES_PACKET_HANDLER(ATHENA_HZ_PRIVATE_MSG_NAME);
      ARES_PACKET_HANDLER(ATHENA_HZ_PING_ACK);
      ARES_PACKET_HANDLER(ARES_HZ_MAP_NAMES);
      ARES_PACKET_HANDLER(ATHENA_HZ_CID_AUTH_FAILED);
      ARES_PACKET_HANDLER(ARES_HZ_CID_AUTH_OK);

    }
  }
}
