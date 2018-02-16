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

        packet::alloc_info allocate(uint16_t& packet_id);
        void dispatch_packet(void* buf, std::function<void(void*)> deallocator);
        
        std::shared_ptr<spdlog::logger> log() const;
        const config& conf() const;

        void rotate_obf_crypt_key();
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
