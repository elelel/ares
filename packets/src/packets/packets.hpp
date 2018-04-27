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
#include "CZ/CZ_REQUEST_TIME.hpp"
#include "ZC/ZC_NOTIFY_TIME.hpp"
#include "ZC/ZC_ACCEPT_ENTER.hpp"
#include "ZC/ZC_REFUSE_ENTER.hpp"
#include "CZ/CZ_LESSEFFECT.hpp"
#include "CZ/CZ_NOTIFY_ACTORINIT.hpp"
#include "CZ/CZ_REQ_SCHEDULER_CASHITEM.hpp"
#include "ZC/ZC_ACK_SCHEDULER_CASHITEM.hpp"
#include "CZ/CZ_REQ_GUILD_MENU.hpp"
#include "CZ/CZ_REQNAME.hpp"
#include "CZ/CZ_REQUEST_MOVE.hpp"
#include "ZC/ZC_NOTIFY_PLAYERMOVE.hpp"
    
    // Account <-> Character
#include "ARES/ARES_HA_LOGIN_REQ.hpp"
#include "ARES/ARES_AH_LOGIN_RESULT.hpp"
#include "ARES/ARES_HA_PING_REQ.hpp"
#include "ARES/ARES_AH_PING_ACK.hpp"
#include "ARES/ARES_HA_ACCOUNT_AUTH_REQ.hpp"
#include "ARES/ARES_AH_ACCOUNT_AUTH_RESULT.hpp"
#include "ARES/ARES_AH_KICK_ACCOUNT.hpp"
#include "ARES/ARES_HA_ONLINE_ACCOUNTS.hpp"
#include "ARES/ARES_HA_USER_COUNT.hpp"

    // Character <-> Zone
#include "ARES/ARES_ZH_LOGIN_REQ.hpp"
#include "ARES/ARES_HZ_LOGIN_RESULT.hpp"
#include "ARES/ARES_ZH_PING_REQ.hpp"
#include "ARES/ARES_HZ_PING_ACK.hpp"
#include "ARES/ARES_ZH_MAP_IDS_REQ.hpp"
#include "ARES/ARES_ZH_MAP_ID_LOADED.hpp"
#include "ARES/ARES_HZ_MAP_IDS.hpp"
#include "ARES/ARES_ZH_CHAR_AUTH_REQ.hpp"
#include "ARES/ARES_HZ_CHAR_AUTH_RESULT.hpp"
#include "ARES/ARES_HZ_PRIVATE_MSG_NAME.hpp"
#include "ARES/ARES_ZH_GAME_RATES.hpp"
    
  }
}

#pragma pack(pop)
