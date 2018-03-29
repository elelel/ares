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

        std::tuple<size_t, size_t, size_t> packet_sizes(const uint16_t packet_id);
        void dispatch_packet(std::shared_ptr<std::byte[]> buf);
        
        std::shared_ptr<spdlog::logger> log() const;
        const config& conf() const;

        int32_t auth_code1{0};
        int32_t auth_code2{0};
      private:
        server& server_;
        session& session_;
      };
      
      ARES_DECLARE_PACKET_HANDLER_TEMPLATE();
      
      ARES_PACKET_HANDLER(ARES_ZH_LOGIN_REQ);
      ARES_PACKET_HANDLER(CH_ENTER);

    }
  }
}
