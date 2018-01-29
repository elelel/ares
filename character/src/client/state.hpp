#pragma once

#include <ares/network>
#include <ares/packets>

#include "../mono/state.hpp"
#include "../database/database.hpp"

namespace ares {
  namespace character {
    namespace client {
      struct state {
        state(character::state& state, session& sess);        
        state(const mono::state& mono_state);
        
        // Interface with character::session
        void defuse_asio();
        void on_open();
        void before_close();
        void on_connection_reset();
        void on_eof();
        void on_socket_error();
        void on_operation_aborted();
        size_t dispatch(const uint16_t PacketType);

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

        /*! Character info for active session */
        std::optional<db::record::character_info> char_info;
        
      private:
        character::state& server_state_;
        session& session_;
      };
      
      ARES_DECLARE_PACKET_HANDLER_TEMPLATE();

      // Simple packet handlers that do not define their own class structure
      ARES_SIMPLE_PACKET_HANDLER(PING);
      ARES_SIMPLE_PACKET_HANDLER(CH_MAKE_CHAR);
      ARES_SIMPLE_PACKET_HANDLER(CH_SELECT_CHAR);
      ARES_SIMPLE_PACKET_HANDLER(CH_CHAR_PAGE_REQ);
      
      // Packet handlers that store state/structured
        
    }
  }
}
