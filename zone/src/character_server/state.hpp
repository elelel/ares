#pragma once

#include <spdlog/spdlog.h>

#include <ares/network>

#include "../predeclare.hpp"

#include "timers.hpp"

namespace ares {
  namespace zone {
    namespace character_server {
      struct state {
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

      public:
        timer::reconnect reconnect_timer;
        timer::ping_request ping_request_timer;
        timer::ping_timeout ping_timeout_timer;
      };
    }
  }
}
