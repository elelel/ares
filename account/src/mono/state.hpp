#pragma once

#include <spdlog/spdlog.h>

#include <ares/network>

#include "../predeclare.hpp"

namespace ares {
  namespace account {
    namespace mono {
      struct state {
        friend struct client::state;
        friend struct char_server::state;
        
        state(std::shared_ptr<spdlog::logger> log, server& serv, session& sess);
        // Interface with account::session
        void defuse_asio();
        void on_open();
        void before_close();
        void on_connection_reset();
        void on_eof();
        void on_socket_error();
        void on_operation_aborted();
        size_t dispatch(const uint16_t PacketType);

        // Data
        std::optional<std::array<uint8_t, 16>> client_exe_hash;

      private:
        std::shared_ptr<spdlog::logger> log_;
        server& server_;
        session& session_;
      };
    }
  }
}
