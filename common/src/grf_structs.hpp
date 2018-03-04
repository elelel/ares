#pragma once

namespace ares {
  namespace grf {
    namespace structs {
      
#pragma pack(push, 1)
      struct grf_header {
        char signature[16];
        char key[14];
        uint64_t filetable_offset;
        uint32_t flags;
        uint32_t version;
      };

      struct filetable_header {
        uint32_t compressed_sz;
        uint32_t uncompressed_sz;
      };

      struct filetable_entry_info {
        uint32_t compressed_sz{0};
        uint32_t compressed_sz_aligned{0};
        uint32_t uncompressed_sz{0};
        uint8_t type{0};
        uint32_t offset{0};
      };
#pragma pack(pop)
      
      static_assert(sizeof(grf_header) == 0x2e, "grf_header structure definition error");
    }
  }
}


