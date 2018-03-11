#include <string>
#include <set>

#include <ares/model>

#pragma pack(push, 1)

namespace ares {
  namespace packet {

#include "CHARACTER_INFO.hpp"
    
    // Servers <-> Client
#include "SC/SC_NOTIFY_BAN.hpp"
#include "CS/PING.hpp"

    // Account <-> Client
#include "CA/CA_EXE_HASHCHECK.hpp"
#include "CA/CA_SSO_LOGIN_REQ.hpp"
#include "AC/AC_ACCEPT_LOGIN.hpp"
#include "AC/AC_REFUSE_LOGIN.hpp"

    // Character <-> Client
#include "CH/CH_ENTER.hpp"
#include "CH/CH_SELECT_CHAR.hpp"
#include "HC/HC_REFUSE_ENTER.hpp"
#include "HC/HC_ACCEPT_ENTER.hpp"
#include "HC/HC_BLOCK_CHARACTER.hpp"
#include "HC/HC_CHAR_PAGES_NUM.hpp"
#include "CH/CH_CHAR_PAGE_REQ.hpp"
#include "HC/HC_CHAR_PAGES.hpp"
#include "CH/CH_MAKE_CHAR.hpp"
#include "HC/HC_ACCEPT_MAKECHAR.hpp"
#include "HC/HC_NOTIFY_ZONESVR.hpp"

    // Zone <-> Client
#include "CZ/CZ_ENTER.hpp"
#include "CZ/CZ_REQUEST_MOVE.hpp"
#include "ZC/ZC_ACCEPT_ENTER.hpp"
#include "ZC/ZC_REFUSE_ENTER.hpp"

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
#include "ATHENA/ATHENA_ZH_CID_AUTH_REQ.hpp"
#include "ATHENA/ATHENA_HZ_CID_AUTH_FAILED.hpp"
    
#include "ARES/ARES_ZH_MAP_IDS_REQ.hpp"
#include "ARES/ARES_ZH_MAP_ID_LOADED.hpp"
#include "ARES/ARES_HZ_MAP_IDS.hpp"
#include "ARES/ARES_HA_AID_AUTH_REQ.hpp"
#include "ARES/ARES_AH_AID_AUTH_RESULT.hpp"
#include "ARES/ARES_ZH_CID_AUTH_REQ.hpp"
#include "ARES/ARES_HZ_CID_AUTH_RESULT.hpp"

  }
}

#pragma pack(pop)
