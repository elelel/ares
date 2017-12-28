#pragma once

#include <spdlog/spdlog.h>

#include <ares/network>

#include "../predeclare.hpp"

namespace ares {
  namespace account {
    namespace client {
      /*! State for sessions that are game clients */
      struct state {
        state(std::shared_ptr<spdlog::logger> log, server& serv, session& sess);
        /*! Constructor to create the client state from monostate
          \param mono_state monostate to convert to client state  */
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
        /*! Game client's executable hash */
        std::optional<std::array<uint8_t, 16>> client_exe_hash;
        /*! Game client's version as reported by client */
        uint32_t version{0};
        /*! Game client's type as reported by client */
        uint8_t clienttype{0};
        /*! Account id */
        uint32_t aid{0};
        /*! Account level */ 
        uint32_t account_level{0};
        /*! Athena-style auth_code, part 1 */
        int32_t auth_code1{0};
        /*! Athena-stule auth_code, part 2 */
        int32_t auth_code2{0};
        /*! Account sex flag */
        uint8_t sex{0};
        /*! Is account online */
        bool online{false};

      };
    }
  }
}
