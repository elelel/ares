#pragma once

namespace ares {
  namespace packet_sets {
    /*! Packet set for client 2017-12-27aRagexeRE.exe
     */
    ARES_PACKETSET_DECLARE(pets_5a3cb64a, client_stable);

#define ARES_PCK_ID_NAME(ID, NAME) ARES_PACKETSET_ID_NAME(pets_5a3cb64a, ID, NAME)
    ARES_PCK_ID_NAME(0x9a1, CH_CHAR_PAGE_REQ);
#undef ARES_PCK_ID_NAME
    
  }
}
