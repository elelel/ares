#pragma once

#include <ares/packets>

#include "../config.hpp"

namespace ares {
  namespace account {
    struct server;
    struct session;

    namespace mono {
      struct state;
    }
    
    namespace client {
      struct state {
        state(account::server& serv, session& sess);
        /*! Constructor to create the client state from monostate
          \param mono_state monostate to convert to client state  */
        state(const mono::state& mono_state);
        
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
        
      private:
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
        model::account_id account_id = model::account_id::from_uint32(0);
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
