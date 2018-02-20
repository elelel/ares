#pragma once

#include <ares/packets>

#include "../config.hpp"

namespace ares {
  namespace zone {
    struct server;
    struct session;

    namespace mono {
      struct state;
    }
    
    namespace client {
      struct state {
        state(zone::server& serv, session& sess);
        /*! Constructor to create the client state from monostate
          \param mono_state monostate to convert to client state  */
        state(const mono::state& mono_state);

        void on_connect();
        void on_connection_reset();
        void on_operation_aborted();
        void on_eof();
        void on_socket_error();
        void defuse_asio();

        packet::alloc_info allocate(uint16_t& packet_id);
        void dispatch_packet(void* buf, std::function<void(void*)> deallocator);
        
        std::shared_ptr<spdlog::logger> log() const;
        const config& conf() const;
        
        void rotate_obf_crypt_key();
        
        // Data
        /*! Account ID */
        uint32_t aid{0};
        /*! Character ID */
        uint32_t cid{0};
        /*! Athena auth code 1 */
        int32_t auth_code1{0};
        /*! Athena auth code 2 (original userLevel) */
        int32_t auth_code2{0};

        int32_t client_time_diff{0};
      private:
        server& server_;
        session& session_;
        std::optional<uint32_t> obf_crypt_key_;
      };
    }
  }
}
