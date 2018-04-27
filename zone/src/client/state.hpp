#pragma once

#include <ares/network>
#include <ares/packets>

#include "../config.hpp"

namespace ares {
  namespace zone {
    struct server;
    struct session;
    struct pc;

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
        
        std::tuple<size_t, size_t, size_t> packet_sizes(uint16_t& packet_id);
        void dispatch_packet(std::shared_ptr<std::byte[]> buf);
        
        std::shared_ptr<spdlog::logger> log() const;
        const config& conf() const;
        
        void rotate_obf_crypt_key();
        
        // Data
        /*! Account ID */
        model::account_id account_id = model::account_id::from_uint32(0);
        /*! Character ID */
        model::character_id character_id = model::character_id::from_uint32(0);
        /*! Athena auth code 1 */
        int32_t auth_code1{0};
        /*! Athena auth code 2 (original userLevel) */
        int32_t auth_code2{0};

        int32_t client_time_diff{0};

        uint32_t tick{0};

        std::weak_ptr<zone::pc> pc;
      private:
        server& server_;
        session& session_;
        std::optional<uint32_t> obf_crypt_key_;
      };

      ARES_DECLARE_PACKET_HANDLER_TEMPLATE();

      ARES_PACKET_HANDLER(CZ_LESSEFFECT);
      ARES_PACKET_HANDLER(CZ_NOTIFY_ACTORINIT);
      ARES_PACKET_HANDLER(CZ_REQUEST_TIME);
      ARES_PACKET_HANDLER(CZ_REQ_SCHEDULER_CASHITEM);
      ARES_PACKET_HANDLER(CZ_REQ_GUILD_MENU);
      ARES_PACKET_HANDLER(CZ_REQNAME);
      ARES_PACKET_HANDLER(CZ_REQUEST_MOVE);
    }
  }
}
