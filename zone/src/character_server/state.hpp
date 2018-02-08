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
        void on_packet_processed();
        
        size_t dispatch_packet(const uint16_t packet_id);

        std::shared_ptr<spdlog::logger> log() const;
        const config& conf() const;
        
      private:
        server& server_;
        session& session_;

        std::atomic<bool> awaiting_ping_ack_{false};
        rxcpp::rxsub::subject<bool> ping_stream;
      };

      ARES_DECLARE_PACKET_HANDLER_TEMPLATE();
      
      // Simple packet handlers that do not define their own class structure
      ARES_SIMPLE_PACKET_HANDLER(ATHENA_HZ_LOGIN_RESULT);
      ARES_SIMPLE_PACKET_HANDLER(ATHENA_HZ_PRIVATE_MSG_NAME);
      ARES_SIMPLE_PACKET_HANDLER(ATHENA_HZ_PING_ACK);
      ARES_SIMPLE_PACKET_HANDLER(ARES_HZ_MAP_NAMES);
      // Packet handlers that store state/structured

    }
  }
}
