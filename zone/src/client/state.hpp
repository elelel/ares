#pragma once

#include "../config.hpp"

namespace ares {
  namespace zone {
    struct server;
    struct session;
    
    namespace client {
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
        
        // Data
        /*! Account ID */
        uint32_t aid{0};
        /*! Character ID */
        uint32_t cid{0};
        /*! Athena auth code 1 */
        int32_t auth_code1{0};
        /*! Athena auth code 2 (original userLevel) */
        int32_t auth_code2{0};
      private:
        server& server_;
        session& session_;
      };
    }
  }
}
