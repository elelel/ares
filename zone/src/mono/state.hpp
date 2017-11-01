#pragma once

#include <spdlog/spdlog.h>

#include <ares/network>

#include "../predeclare.hpp"
#include "../character_server/state.hpp"
#include "../client/state.hpp"

namespace ares {
  namespace zone {
    namespace mono {
      struct state {
        friend struct client::state;
        friend struct character_server::state;
        
        state(std::shared_ptr<spdlog::logger> log, server& serv, session& sess);        
        // Interface with zone::session
        void on_open();
        void before_close();
        void on_connection_reset();
        void on_eof();
        void on_socket_error();
        void on_operation_aborted();
        size_t dispatch(const uint16_t PacketType);

        void defuse_asio();
      private:
        std::shared_ptr<spdlog::logger> log_;
        server& server_;
        session& session_;
      };
    }
  }
}
