#pragma once

namespace ares {
  namespace packet_sets {
    /*! Packet set for client 2017-12-27aRagexeRE.exe
     */
    ARES_PACKETSET_DECLARE(pets_5a3cb64a, client_stable);

#define ARES_PCK_ID_NAME(ID, NAME) ARES_PACKETSET_ID_NAME(pets_5a3cb64a, ID, NAME)
#define ARES_PCK_CHOOSE_VERSION(NAME, VERSION_NAME) ARES_PACKETSET_CHOOSE_VERSION(pets_5a3cb64a, NAME, VERSION_NAME)

    ARES_PCK_ID_NAME(0x9a0, HC_CHAR_PAGES_NUM::with_nchars);
    ARES_PCK_CHOOSE_VERSION(HC_CHAR_PAGES_NUM, HC_CHAR_PAGES_NUM::with_nchars);
    
#undef ARES_PCK_CHOOSE_VERSION    
#undef ARES_PCK_ID_NAME
    
  }
}
