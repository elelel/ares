#pragma once

#include <spdlog/spdlog.h>

#include <ares/network>

#include "../predeclare.hpp"

namespace ares {
  namespace character {
    namespace client {
      struct state {
        state(std::shared_ptr<spdlog::logger> log, server& serv, session& sess);        
        state(const mono::state& mono_state);
        
        // Interface with character::session
        void defuse_asio();
        void on_open();
        void before_close();
        void on_connection_reset();
        void on_eof();
        void on_socket_error();
        void on_operation_aborted();
        size_t dispatch(const uint16_t PacketType);

        // Data
        uint32_t aid{0};
        /*! Athena auth code 1 */
        int32_t auth_code1{0};
        /*! Athena auth code 2 (original userLevel) */
        int32_t auth_code2{0};
      private:
        std::shared_ptr<spdlog::logger> log_;
        server& server_;
        session& session_;
      };
    }
  }
}
