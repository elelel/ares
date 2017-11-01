#pragma once

#include <spdlog/spdlog.h>

#include <ares/network>

#include "../predeclare.hpp"

namespace ares {
  namespace account {
    namespace client {
      struct state {
        
        state(std::shared_ptr<spdlog::logger> log, server& serv, session& sess);
        state(const mono::state& mono_state);
        
        // Interface with account::session
        void defuse_asio();
        void on_open();
        void before_close();
        void on_connection_reset();
        void on_eof();
        void on_socket_error();
        void on_operation_aborted();
        size_t dispatch(const uint16_t PacketType);

      private:
        std::shared_ptr<spdlog::logger> log_;
        server& server_;
        session& session_;

      public:
        // Data
        std::optional<std::array<uint8_t, 16>> client_exe_hash;
        uint32_t version{0};
        uint8_t clienttype{0};
        uint32_t aid{0};
        uint32_t account_level{0};
        int32_t auth_code1{0};
        int32_t auth_code2{0};
        uint8_t sex{0};
        bool online{false};

      };
    }
  }
}
