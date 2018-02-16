#pragma once

#include <ares/network>
#include <ares/packets>

#include "../config.hpp"
#include "../database/database.hpp"

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

        void on_connect();
        void on_connection_reset();
        void on_operation_aborted();
        void on_eof();
        void on_socket_error();
        void defuse_asio();

        packet::alloc_info allocate(const uint16_t packet_id);
        void dispatch_packet(void* buf, std::function<void(void*)> deallocator);
        
        std::shared_ptr<spdlog::logger> log() const;
        const config& conf() const;

        // Data
        uint32_t aid{0};
        uint32_t cid{0};
        /*! Athena auth code 1 */
        int32_t auth_code1{0};
        /*! Athena auth code 2 (original userLevel) */
        int32_t auth_code2{0};
        /*! Expiration time in epoch format */
        uint32_t expiration_time{0};
        /*! User's email */
        std::string email;
        /*! User's pin */
        std::string pin;
        /*! GM level */
        uint8_t gmlevel{0};

        size_t playable_slots{0};
        size_t creatable_slots{0};
        /*! Character info for char select */
        std::vector<db::record::character_info> char_select_character_info;
        bool char_select_all_sent{false};
        size_t char_page_reqs{0};

        /*! Character info for active session */
        std::optional<db::record::character_info> char_info;
        
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
