#pragma once

/*! \file packet_ids.hpp
  \brief Packet ids for base packet version

*/

namespace ares {
  template <>
  struct packet_ids<packet_version::base> {
    // Server <-> Client
    static const uint16_t SC_NOTIFY_BAN = 0x81;

    // Account <-> Client
    static const uint16_t AC_REFUSE_LOGIN = 0x6a;
    static const uint16_t CA_EXE_HASHCHECK = 0x204;
    static const uint16_t CA_SSO_LOGIN_REQ_0x64 = 0x64;
    static const uint16_t CA_SSO_LOGIN_REQ = 0x825;
    static const uint16_t AC_ACCEPT_LOGIN = 0xac4;
    // Character <-> Client
    static const uint16_t CH_ENTER = 0x65;
    static const uint16_t CH_MAKE_CHAR = 0x67;
    static const uint16_t CH_MAKE_CHAR_0x970 = 0x970;
    static const uint16_t CH_MAKE_CHAR_0xa39 = 0xa39;
    static const uint16_t CH_DELETE_CHAR = 0x68;
    static const uint16_t HC_ACCEPT_ENTER_0x6b = 0x6b;
    static const uint16_t HC_ACCEPT_ENTER = 0x82d;
    static const uint16_t HC_CHAR_PAGES_NUM = 0x9a0;
    static const uint16_t HC_CHAR_PAGES = 0x99d;
    static const uint16_t HC_REFUSE_ENTER = 0x6c;
    static const uint16_t HC_ACCEPT_MAKECHAR = 0x6d;
    static const uint16_t HC_REFUSE_MAKECHAR = 0x6e;

    // Zone <-> Client
    static const uint16_t CZ_REQUEST_MOVE = 0x85;

    // Athena packets
    // Account <-> Character
    static const uint16_t ATHENA_HA_LOGIN_REQ = 0x2710;
    static const uint16_t ATHENA_AH_LOGIN_RESULT = 0x2711;
    static const uint16_t ATHENA_HA_PING_REQ = 0x2719;
    static const uint16_t ATHENA_AH_PING_ACK = 0x2718;
    static const uint16_t ATHENA_HA_AID_AUTH_REQ = 0x2712;
    static const uint16_t ATHENA_AH_AID_AUTH_RESULT = 0x2713;
    static const uint16_t ATHENA_HA_USER_COUNT = 0x2714;
    static const uint16_t ATHENA_HA_ACCOUNT_DATA_REQ = 0x2716;
    static const uint16_t ATHENA_AH_ACCOUNT_DATA_RESULT = 0x2717;
    static const uint16_t ATHENA_HA_SET_AID_ONLINE = 0x272b;
    static const uint16_t ATHENA_HA_SET_AID_OFFLINE = 0x272c;
    static const uint16_t ATHENA_HA_ONLINE_AIDS = 0x272d;
    static const uint16_t ATHENA_AH_KICK_AID = 0x2734;
    static const uint16_t ATHENA_HA_SET_ALL_AIDS_OFFLINE = 0x2737;

    // Character <-> Zone
    static const uint16_t ATHENA_ZH_LOGIN_REQ = 0x2af8;
    static const uint16_t ATHENA_HZ_LOGIN_RESULT = 0x2af9;
    static const uint16_t ATHENA_ZH_MAP_NAMES = 0x2afa;
    static const uint16_t ATHENA_HZ_PRIVATE_MSG_NAME = 0x2afb;
    static const uint16_t ATHENA_ZH_PING_REQ = 0x2b23;
    static const uint16_t ATHENA_HZ_PING_ACK = 0x2b24;
    static const uint16_t ATHENA_ZH_ONLINE_USERS = 0x2aff;
    static const uint16_t ATHENA_ZH_USER_COUNT = 0x2afe;
    static const uint16_t ATHENA_ZH_GAME_RATES = 0x2b16;
  };
}
