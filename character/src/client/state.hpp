#pragma once

#include <ares/database>
#include <ares/network>
#include <ares/packets>

#include "../config.hpp"


namespace ares {
  namespace character {
    struct server;
    struct session;

    namespace mono {
      struct state;
    }
    
    namespace client {
      struct state {
        state(server& serv, session& sess);        
        state(const mono::state& mono_state);
        ~state();

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

        // Data
        model::account_id account_id = model::account_id::from_uint32(0);
        /*! Athena auth code 1 */
        int32_t auth_code1{0};
        /*! Athena auth code 2 (original userLevel) */
        int32_t auth_code2{0};

        size_t playable_slots{0};
        size_t creatable_slots{0};
        /*! Character info for char select */
        std::vector<model::pc_info> char_select_character_info;
        bool char_select_all_sent{false};
        size_t char_page_reqs{0};

        /*! Character info for active session */
        std::optional<model::pc_info> char_info;
        
      private:
        server& server_;
        session& session_;
      };
      
      ARES_DECLARE_PACKET_HANDLER_TEMPLATE();

      ARES_PACKET_HANDLER(PING);
      ARES_PACKET_HANDLER(CH_MAKE_CHAR::no_stats);
      ARES_PACKET_HANDLER(CH_SELECT_CHAR);
      ARES_PACKET_HANDLER(CH_CHAR_PAGE_REQ);
      
    }
  }
}
