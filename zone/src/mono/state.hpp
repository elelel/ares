#pragma once

#include <ares/network>
#include <ares/packets>

#include "../config.hpp"

namespace ares {
  namespace zone {
    struct server;
    struct session;

    namespace client {
      struct state;
    }
    
    namespace mono {
      struct state {
        state(zone::server& serv, session& sess);
        
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

        int32_t client_time_diff;
        int32_t auth_code1;
        model::account_id account_id = model::account_id::from_uint32(0);
        model::character_id character_id = model::character_id::from_uint32(0);
      private:
        friend struct zone::client::state;
        
        server& server_;
        session& session_;
        std::optional<uint32_t> obf_crypt_key_;
      };

      ARES_DECLARE_PACKET_HANDLER_TEMPLATE();

      ARES_PACKET_HANDLER(CZ_ENTER);

    }
  }
}
