/*! \file name.hpp
  \brief Packet names (as type traits)
*/

#include "size.hpp"

namespace ares {
  namespace packet {
    enum class KIND {
      UNKNOWN = -1,
      STATIC,
      DYNAMIC
    };
    
    template <typename Packet>
    inline static Packet* copy_construct(const Packet& other) {
      const size_t sz = size::get<Packet>(&other, sizeof(Packet));
      Packet* p = (Packet*)malloc(sz);
      if (p != nullptr) {
        memcpy(p, &other, sz);
        return p;
      } else {
        throw std::bad_alloc();
      }
    }
    
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
    struct CZ_REQUEST_MOVE {};
    struct ZC_ACCEPT_ENTER {};
    struct ZC_REFUSE_ENTER {};

    // Athena packets
    // Account <-> Character
    struct ATHENA_HA_LOGIN_REQ {};
    struct ATHENA_AH_LOGIN_RESULT {};
    struct ATHENA_HA_PING_REQ {};
    struct ATHENA_AH_PING_ACK {};
    struct ATHENA_HA_AID_AUTH_REQ {};
    struct ATHENA_AH_AID_AUTH_RESULT {};
    struct ATHENA_HA_USER_COUNT {};
    struct ATHENA_HA_ACCOUNT_DATA_REQ {};
    struct ATHENA_AH_ACCOUNT_DATA_RESULT {};
    struct ATHENA_HA_SET_AID_ONLINE {};
    struct ATHENA_HA_SET_AID_OFFLINE {};
    struct ATHENA_HA_ONLINE_AIDS {};
    struct ATHENA_AH_KICK_AID {};
    struct ATHENA_HA_SET_ALL_AIDS_OFFLINE {};

    // Character <-> Zone
    struct ATHENA_ZH_LOGIN_REQ {};
    struct ATHENA_HZ_LOGIN_RESULT {};
    struct ATHENA_ZH_MAP_NAMES {};
    struct ATHENA_HZ_PRIVATE_MSG_NAME {};
    struct ATHENA_ZH_PING_REQ {};
    struct ATHENA_HZ_PING_ACK {};
    struct ATHENA_ZH_ONLINE_USERS {};
    struct ATHENA_ZH_USER_COUNT {};
    struct ATHENA_ZH_GAME_RATES {};
    struct ATHENA_ZH_CID_AUTH_REQ {};
    struct ATHENA_HZ_CID_AUTH_FAILED {};

    // Ares
    struct ARES_ZH_MAP_IDS_REQ {};
    struct ARES_HZ_MAP_IDS {};
    struct ARES_HA_AID_AUTH_REQ {};
    struct ARES_AH_AID_AUTH_RESULT {};
    struct ARES_ZH_CID_AUTH_REQ {};
    struct ARES_HZ_CID_AUTH_RESULT {};
    struct ARES_ZH_MAP_ID_LOADED {};
  }
}
