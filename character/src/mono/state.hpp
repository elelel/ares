#pragma once

#include <ares/network>
#include <ares/packets>

#include "../config.hpp"

namespace ares {
  namespace character {
    struct server;
    struct session;

    namespace client {
      struct state;
    }

    namespace zone_server {
      struct state;
    }
    
    namespace mono {
      struct state {
        state(character::server& serv, session& sess);
        ~state();

        friend struct client::state;
        friend struct zone_server::state;

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

        int32_t auth_code1{0};
        int32_t auth_code2{0};
      private:
        server& server_;
        session& session_;
      };
      
      ARES_DECLARE_PACKET_HANDLER_TEMPLATE();
      
      ARES_PACKET_HANDLER(ATHENA_ZH_LOGIN_REQ);
      ARES_PACKET_HANDLER(CH_ENTER);

    }
  }
}
