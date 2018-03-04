#pragma once

/*! \file grf.hpp
 A rewrite of Athena's grfio library for Ares
 */

#include <zlib.h>

#include <vector>
#include <unordered_map>
#include <map>
#include <memory>


namespace ares {
  namespace grf {
    enum class STORAGE {
      LOCAL_DIR,
      GRF
    };

    enum FILELIST_TYPE : uint8_t {
      FILE = 0x01, // entry is a file
      ENCRYPT_MIXED = 0x02, // encryption mode 0 (header DES + periodic DES/shuffle)
      ENCRYPT_HEADER = 0x04 // encryption mode 1 (header DES only)
    };

#pragma pack(push, 1)
#pragma pack(pop)

    struct resource {
      STORAGE storage;
      std::shared_ptr<std::string> source;
      std::string name;
      uint32_t compressed_sz{0};
      uint32_t compressed_sz_aligned{0};
      uint32_t uncompressed_sz{0};
      uint32_t offset{0};
      uint8_t type{0};
    };

    struct resource_set {
      resource_set(const std::vector<std::string>& filenames,
                   const size_t resnametable_idx = 0);
    private:
      std::vector<std::shared_ptr<std::string>> sources;
      std::unordered_map<std::string, std::string> resnametable;
      std::unordered_map<std::string, resource> resources;
      size_t last_source_id{0};

      std::shared_ptr<std::string> get_source(const std::string& source);      
      void add_local_dir(const std::string& dir_path);
      void add_grf(const std::string& grf_path);
      void load_resnametable(const std::string& from_path);
      void parse_resnametable(const std::string& buf);
    };
    
    
  }
}
