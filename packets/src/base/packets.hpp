#pragma once

/*! \file packets.hpp
  \brief Packet structures for base packet version
*/

#include "../utils.hpp"

#pragma pack(push, 1)
namespace ares {
  template <>
  struct packets<packet_version::base> {
    using packet_id = packet_ids<packet_version::base>;

    #include "CHARACTER_INFO.hpp"
    
    // Servers <-> Client
    #include "SC/SC_NOTIFY_BAN.hpp"
    #include "CS/PING.hpp"

    // Account <-> Client
    #include "CA/CA_EXE_HASHCHECK.hpp"
    #include "CA/CA_SSO_LOGIN_REQ_0x64.hpp"
    #include "CA/CA_SSO_LOGIN_REQ.hpp"
    #include "AC/AC_ACCEPT_LOGIN.hpp"
    #include "AC/AC_REFUSE_LOGIN.hpp"

    // Character <-> Client
    #include "CH/CH_ENTER.hpp"
    #include "HC/HC_REFUSE_ENTER.hpp"
    #include "HC/HC_ACCEPT_ENTER.hpp"
    #include "HC/HC_CHAR_PAGES_NUM.hpp"
    #include "CH/CH_CHAR_PAGE_REQ.hpp"
    #include "HC/HC_CHAR_PAGES.hpp"
    #include "CH/CH_MAKE_CHAR_0x67.hpp"
    #include "CH/CH_MAKE_CHAR.hpp"
    #include "HC/HC_ACCEPT_MAKECHAR.hpp"

    // Zone <-> Client
    #include "CZ/CZ_REQUEST_MOVE.hpp"

    // Athena packets
    // Account <-> Character
    #include "ATHENA/ATHENA_HA_PING_REQ.hpp"
    #include "ATHENA/ATHENA_AH_PING_ACK.hpp"
    #include "ATHENA/ATHENA_HA_LOGIN_REQ.hpp"
    #include "ATHENA/ATHENA_AH_LOGIN_RESULT.hpp"
    #include "ATHENA/ATHENA_HA_ACCOUNT_DATA_REQ.hpp"
    #include "ATHENA/ATHENA_AH_ACCOUNT_DATA_RESULT.hpp"
    #include "ATHENA/ATHENA_HA_AID_AUTH_REQ.hpp"
    #include "ATHENA/ATHENA_HA_ONLINE_AIDS.hpp"
    #include "ATHENA/ATHENA_HA_USER_COUNT.hpp"
    #include "ATHENA/ATHENA_HA_SET_AID_ONLINE.hpp"
    #include "ATHENA/ATHENA_HA_SET_AID_OFFLINE.hpp"
    #include "ATHENA/ATHENA_HA_SET_ALL_AIDS_OFFLINE.hpp"
    #include "ATHENA/ATHENA_AH_KICK_AID.hpp"
    #include "ATHENA/ATHENA_AH_AID_AUTH_RESULT.hpp"

    // Character <-> Zone
    #include "ATHENA/ATHENA_ZH_LOGIN_REQ.hpp"
    #include "ATHENA/ATHENA_HZ_LOGIN_RESULT.hpp"
    #include "ATHENA/ATHENA_HZ_PRIVATE_MSG_NAME.hpp"
    #include "ATHENA/ATHENA_ZH_PING_REQ.hpp"
    #include "ATHENA/ATHENA_HZ_PING_ACK.hpp"
    #include "ATHENA/ATHENA_ZH_ONLINE_USERS.hpp"    
    #include "ATHENA/ATHENA_ZH_USER_COUNT.hpp"    
    #include "ATHENA/ATHENA_ZH_GAME_RATES.hpp"    
    #include "ATHENA/ATHENA_ZH_MAP_NAMES.hpp"
  };
}
#pragma pack(pop)
