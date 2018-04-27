/*! \file name.hpp
  \brief Packet names (as type traits)
*/

#include "size.hpp"

namespace ares {
  namespace packet {
    template <typename PacketSet, typename PacketName>
    struct type {
    };

    // Server <-> Client
    struct SC_NOTIFY_BAN {};
    struct PING {};

    // Account <-> Client
    struct AC_REFUSE_LOGIN {};
    struct CA_EXE_HASHCHECK {};
    struct CA_SSO_LOGIN_REQ {
      /*! Simple login/password variant of account server login packet, usually id 0x64 */
      struct login_password {};
      /*! Token based authentication, usually id 0x825 */
      struct token_auth {};
    };
    struct AC_ACCEPT_LOGIN {};
    // Character <-> Client
    struct CH_ENTER {};
    struct CH_SELECT_CHAR {};
    struct CH_MAKE_CHAR {
      /*! Older version, where stats were set by user when creating char */
      struct with_stats {};
      /*! No stats selection on char creation */
      struct no_stats {};
    };
    struct CH_DELETE_CHAR {};
    struct HC_ACCEPT_ENTER_0x6b {};
    struct HC_NOTIFY_ZONESVR {};
    struct HC_BLOCK_CHARACTER {};
    struct HC_ACCEPT_ENTER {};
    struct HC_CHAR_PAGES_NUM {
      struct with_nchars {};
      struct no_nchars{};
    };
    struct CH_CHAR_PAGE_REQ {};
    struct HC_CHAR_PAGES {};
    struct HC_REFUSE_ENTER {};
    struct HC_ACCEPT_MAKECHAR {};
    struct HC_REFUSE_MAKECHAR {};

    // Zone <-> Client
    struct CZ_ENTER {};
    struct CZ_REQUEST_TIME {};
    struct ZC_NOTIFY_TIME {};
    struct CZ_REQUEST_MOVE {};
    struct ZC_NOTIFY_PLAYERMOVE {};
    struct ZC_ACCEPT_ENTER {};
    struct ZC_REFUSE_ENTER {};
    struct CZ_LESSEFFECT {};
    struct CZ_NOTIFY_ACTORINIT {};
    struct CZ_REQ_SCHEDULER_CASHITEM {};
    struct ZC_ACK_SCHEDULER_CASHITEM {};
    struct CZ_REQ_GUILD_MENU {};
    struct CZ_REQNAME {};

    
    // Interserver packets

    // Account <-> Character
    struct ARES_HA_LOGIN_REQ {};
    struct ARES_AH_LOGIN_RESULT {};
    struct ARES_HA_PING_REQ {};
    struct ARES_AH_PING_ACK {};
    struct ARES_HA_ACCOUNT_AUTH_REQ {};
    struct ARES_AH_ACCOUNT_AUTH_RESULT {};
    struct ARES_AH_KICK_ACCOUNT {};
    struct ARES_HA_ONLINE_ACCOUNTS {};
    struct ARES_HA_USER_COUNT {};
    
    // Character <-> Zone
    struct ARES_ZH_LOGIN_REQ {};
    struct ARES_HZ_LOGIN_RESULT {};
    struct ARES_ZH_PING_REQ {};
    struct ARES_HZ_PING_ACK {};
    struct ARES_ZH_MAP_IDS_REQ {};
    struct ARES_HZ_MAP_IDS {};
    struct ARES_ZH_CHAR_AUTH_REQ {};
    struct ARES_HZ_CHAR_AUTH_RESULT {};
    struct ARES_ZH_MAP_ID_LOADED {};
    struct ARES_HZ_PRIVATE_MSG_NAME {};
    struct ARES_ZH_GAME_RATES {};
  }
}
