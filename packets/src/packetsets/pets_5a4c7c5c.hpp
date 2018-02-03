#pragma once

namespace ares {
  namespace packet_sets {
    /*! Packet set for client 2018-01-03bRagexeRE.exe
     */
    ARES_PACKETSET_DECLARE(pets_5a4c7c5c, client_stable);

#define ARES_PCK_ID_NAME(ID, NAME) ARES_PACKETSET_ID_NAME(pets_5a4c7c5c, ID, NAME)

    ARES_PCK_ID_NAME(0x9a0, HC_CHAR_PAGES_NUM::no_nchars);
    
#undef ARES_PCK_ID_NAME
  }
}

