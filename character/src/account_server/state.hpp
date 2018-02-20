#pragma once

#include <ares/network>
#include <ares/packets>

#include "../config.hpp"

namespace ares {
  namespace character {
    struct server;
    struct session;
    
    namespace account_server {
      struct state {
        state(server& serv, session& sess);

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

        void reset_ping_account_server_timer();
      private:
        server& server_;
        session& session_;

      public:
        std::shared_ptr<asio::steady_timer> ping_account_server_timer_;

        //timer::send_aids send_aids_timer;
        //timer::send_user_count send_user_count_timer;
      };
      
      ARES_DECLARE_PACKET_HANDLER_TEMPLATE();

      ARES_PACKET_HANDLER(ATHENA_AH_LOGIN_RESULT);
      ARES_PACKET_HANDLER(ATHENA_AH_PING_ACK);
      ARES_PACKET_HANDLER(ARES_AH_AID_AUTH_RESULT);
      ARES_PACKET_HANDLER(ATHENA_AH_ACCOUNT_DATA_RESULT);
      ARES_PACKET_HANDLER(ATHENA_AH_KICK_AID);
    }
  }
}
