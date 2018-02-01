#pragma once

#include <spdlog/spdlog.h>

#include <ares/network>
#include <ares/packets>

#include "../mono/state.hpp"

namespace ares {
  namespace zone {
    struct state;
    struct server;
    
    namespace client {
      struct state {
        state(zone::state& server_state, session& sess);        
        state(const mono::state& mono_state);
        
        // Interface with zone::session
        void defuse_asio();
        void on_open();
        void before_close();
        void on_connection_reset();
        void on_eof();
        void on_socket_error();
        void on_operation_aborted();
        size_t dispatch(const uint16_t PacketType);

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
        zone::state& server_state_;
        session& session_;
      };

      ARES_DECLARE_PACKET_HANDLER_TEMPLATE(zone);
      
      // Simple packet handlers that do not define their own class structure
      ARES_SIMPLE_PACKET_HANDLER(zone, CZ_REQUEST_MOVE);
      
      // Packet handlers that store state/structured

    }
  }
}
