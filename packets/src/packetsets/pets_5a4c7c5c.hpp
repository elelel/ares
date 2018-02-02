#pragma once

namespace ares {
  namespace packet_sets {
    /*! Packet set for client 2018-01-03bRagexeRE.exe
     */
    struct pets_5a4c7c5c : server {     // TODO: Make chain inheritance
      template <uint16_t PacketId> struct name_of : server::name_of<PacketId> {
        using type = typename server::name_of<PacketId>::type;
      };
      
      template <typename T> struct id_of :
        server::id_of<ares::packets::type<packet_sets::server,
                                    typename T::packet_name>> {
      };
    };

#define ARES_PCK_ID_TYPE(ID, TYPE) ARES_PACKETSET_ID_TYPE(pets_5a4c7c5c, ID, TYPE)

    ARES_PCK_ID_TYPE(0x81, SC_NOTIFY_BAN);
    ARES_PCK_ID_TYPE(0x187, PING);

    // Account <-> Client
    ARES_PCK_ID_TYPE(0x6a, AC_REFUSE_LOGIN);
    ARES_PCK_ID_TYPE(0x204, CA_EXE_HASHCHECK);
    ARES_PCK_ID_TYPE(0x64, CA_SSO_LOGIN_REQ::login_password);
    ARES_PCK_ID_TYPE(0x825, CA_SSO_LOGIN_REQ::token_auth);
    ARES_PCK_ID_TYPE(0xac4, AC_ACCEPT_LOGIN);
    // Character <-> Client
    ARES_PCK_ID_TYPE(0x65, CH_ENTER);
    ARES_PCK_ID_TYPE(0x66, CH_SELECT_CHAR);
    ARES_PCK_ID_TYPE(0xa39, CH_MAKE_CHAR::no_stats);
    ARES_PCK_ID_TYPE(0x68, CH_DELETE_CHAR);
    ARES_PCK_ID_TYPE(0x71, HC_NOTIFY_ZONESVR);
    ARES_PCK_ID_TYPE(0x20d, HC_BLOCK_CHARACTER);
    ARES_PCK_ID_TYPE(0x82d, HC_ACCEPT_ENTER);
    ARES_PCK_ID_TYPE(0x9a0, HC_CHAR_PAGES_NUM);
    ARES_PCK_ID_TYPE(0x9a1, CH_CHAR_PAGE_REQ);
    ARES_PCK_ID_TYPE(0x99d, HC_CHAR_PAGES);
    ARES_PCK_ID_TYPE(0x6c, HC_REFUSE_ENTER);
    ARES_PCK_ID_TYPE(0x6d, HC_ACCEPT_MAKECHAR);
    ARES_PCK_ID_TYPE(0x6e, HC_REFUSE_MAKECHAR);

    // Zone <-> Client
    ARES_PCK_ID_TYPE(0x85, CZ_REQUEST_MOVE);

#undef ARES_PCK_ID_TYPE

  }
}

