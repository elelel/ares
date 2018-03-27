#pragma once

/*! \file grf.hpp
 A rewrite of Athena's grfio library for Ares
 */

#include <map>
#include <memory>
#include <optional>
#include <unordered_map>
#include <vector>

namespace ares {
  namespace grf {
    /*! Type of storage (is resource stored as file in a local dir or in grf) */
    enum class STORAGE {
      LOCAL_DIR,
      GRF
    };

    /*! Encryption mode for GRF
     */
    enum FILELIST_TYPE : uint8_t {
      FILE = 0x01, // entry is a file
      ENCRYPT_MIXED = 0x02, // encryption mode 0 (header DES + periodic DES/shuffle)
      ENCRYPT_HEADER = 0x04 // encryption mode 1 (header DES only)
    };

#pragma pack(push, 1)
#pragma pack(pop)

    /*!
      Grf/dir resource descriptor
     */
    struct resource {
      STORAGE storage;
      std::shared_ptr<std::string> source;
      std::string name;
      uint32_t compressed_sz{0};
      uint32_t compressed_sz_aligned{0};
      uintmax_t uncompressed_sz{0};
      uint32_t offset{0};
      uint8_t type{0};
    };

    /*!
      Container to catalog grf/dir resources
     */
    struct resource_set {
      /*! \param filenames file or directory names as sources to search for resources in */
      resource_set(const std::vector<std::string>& filenames);
      /*! Find resource file in given source */
      std::optional<std::string> find_resource_file(const std::string& src);
      /*! Read resource content resolving dir or grf source */
      std::optional<std::vector<uint8_t>> read_file(const std::string& pathfn);

    private:
      std::vector<std::shared_ptr<std::string>> sources;
      std::unordered_map<std::string, std::string> resnametable;
      std::unordered_map<std::string, std::vector<resource>> resources;
      size_t last_source_id{0};

      std::shared_ptr<std::string> get_source(const std::string& source);      
      void add_local_dir(const std::string& dir_path);
      void add_grf(const std::string& grf_path);
      void parse_resnametable(const std::string& buf);
      std::optional<std::vector<uint8_t>> read_file_local_dir(const resource& r);
      std::optional<std::vector<uint8_t>> read_file_grf(const resource& r);
      void decode_grf(std::vector<uint8_t>& compressed, FILELIST_TYPE type, uint32_t compressed_sz) const;
      void decrypt_grf_shuffle(uint64_t& src) const;
    };
    
    
  }
}
