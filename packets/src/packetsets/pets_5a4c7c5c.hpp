#pragma once

namespace ares {
  namespace packet_sets {
    /*! Packet set for client 2018-01-03bRagexeRE.exe
     */
    ARES_PACKETSET_DECLARE(pets_5a4c7c5c, client_stable);

#define ARES_PCK_ID_NAME(ID, NAME) ARES_PACKETSET_ID_NAME(pets_5a4c7c5c, ID, NAME)

    ARES_PCK_ID_NAME(0x811, CZ_ENTER);
    ARES_PCK_ID_NAME(0x9a0, HC_CHAR_PAGES_NUM::no_nchars);
    ARES_PCK_ID_NAME(0x876, CZ_REQUEST_TIME);
    ARES_PCK_ID_NAME(0x8ab, CZ_REQNAME);
    ARES_PCK_ID_NAME(0x941, CZ_REQUEST_MOVE);

#undef ARES_PCK_ID_NAME
  }
}

