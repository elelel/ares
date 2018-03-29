#include "grf.hpp"

#include <algorithm>
#include <experimental/filesystem>
#include <fstream>
#include <string_view>
#include <system_error>
#include <sstream>
#include <type_traits>

#include <zlib.h>
#include <LzmaDec.h>

#include "des.hpp"
#include "grf_structs.hpp"


ares::grf::resource_set::resource_set(const std::vector<std::string>& filenames) {
  for (const auto& filename : filenames) {
    std::error_code ec;
    namespace fs = std::experimental::filesystem;
    auto filepath = fs::path(filename);
    if (fs::exists(filepath, ec) && (ec.value() == 0)) {
      if (fs::is_directory(filepath)) {
        add_local_dir(filepath.string());
      } else if (fs::is_regular_file(filepath) && (filepath.extension() == ".grf")) {
        add_grf(filepath.string());
      }
    } else {
      throw std::runtime_error("ares::grf path " + filename + " does not exist or error determining file existence");
    }
  }
}

std::shared_ptr<std::string> ares::grf::resource_set::get_source(const std::string& source) {
  auto found = std::find_if(sources.begin(), sources.end(), [&source] (const std::shared_ptr<std::string>& s) {
      return *s == source;
    });
  if (found != sources.end()) {
    return *found;
  } else {
    auto new_source = std::make_shared<std::string>(source);
    sources.push_back(new_source);
    return new_source;
  }
}

void ares::grf::resource_set::add_local_dir(const std::string& dir_path) {
  namespace fs = std::experimental::filesystem;
  for (const auto& p : fs::recursive_directory_iterator(dir_path)) {
    if (fs::is_regular_file(p)) {
      std::string path = p.path().string();
      resource f;
      f.storage = STORAGE::LOCAL_DIR;
      f.source = get_source(dir_path);
      f.name = path;
      f.uncompressed_sz = fs::file_size(p.path());

      if (f.name == "data\\resnametable.txt") {
        std::ifstream file(path, std::ios::binary | std::ios::ate);
        std::streamsize sz = file.tellg();
        std::string text(sz, '\0');
        file.read(text.data(), sz);
        parse_resnametable(text);
      }
      resources[path].push_back(std::move(f));
    }
  }
}

void ares::grf::resource_set::add_grf(const std::string& grf_path) {
  namespace fs = std::experimental::filesystem;
  auto grf_sz = fs::file_size(grf_path);
  std::ifstream f(grf_path);
  if (f) {
    structs::grf_header grf_header;
    f.read((char*)&grf_header, sizeof(grf_header));
    if (std::string("Master of Magic") == grf_header.signature) {
      if (grf_header.version == 0x0200) {
        structs::filetable_header filetable_header;
        f.seekg(grf_header.filetable_offset, std::ios_base::cur);
        f.read((char*)&filetable_header, sizeof(filetable_header));
        if (filetable_header.compressed_sz <= grf_sz - f.tellg()) {
          std::vector<uint8_t> compressed(filetable_header.compressed_sz);
          std::vector<uint8_t> uncompressed(filetable_header.uncompressed_sz);
          f.read((char*)compressed.data(), compressed.size());
          auto actual_sz = uLongf(uncompressed.size());
          auto zlib_rc = uncompress((Bytef*)uncompressed.data(), &actual_sz, (const Bytef*)compressed.data(), uLong(compressed.size()));
          if ((zlib_rc == Z_OK) && (actual_sz == uncompressed.size())) {
            size_t ofs = 0;
            while (ofs < uncompressed.size()) {
              resource r;
              r.storage = STORAGE::GRF;
              r.source = get_source(grf_path);
              std::string fname((char*)&uncompressed[ofs]);
              // GRF is case-ignorant
              std::transform(fname.begin(), fname.end(), fname.begin(), ::tolower);
              r.name = fname;
              auto info = (structs::filetable_entry_info*)&uncompressed[ofs + r.name.size() + 1];
              r.compressed_sz = info->compressed_sz;
              r.compressed_sz_aligned = info->compressed_sz_aligned;
              r.uncompressed_sz = info->uncompressed_sz;
              r.offset = info->offset + sizeof(grf_header);
              r.type = info->type;
              ofs += r.name.size() + 1 + sizeof(structs::filetable_entry_info);
              resources[fname].push_back(std::move(r));
              if (fname == "data\\resnametable.txt") {
                auto buf = read_file(fname);
                if (buf) {
                  std::string text(buf->begin(), buf->end());
                  parse_resnametable(text);
                } else {
                  throw std::runtime_error("ares::grf resnametable could not be read from " + grf_path);
                }
              }
            }
          } else {
            throw std::runtime_error("ares::grf uncompressed filetable size mismatch");
          }
        } else {
          throw std::runtime_error("ares::grf error in extended header, compressed size is greater than file remainder in " + grf_path);
        }
      } else {
        throw std::runtime_error("ares::grf unsupported grf version " + std::to_string(grf_header.version) + " in " + grf_path);
      }
    } else {
      throw std::runtime_error("ares::grf bad header signature in " + grf_path);
    }
  }
}

std::optional<std::vector<uint8_t>> ares::grf::resource_set::read_file(const std::string& pathfn) {
  std::optional<std::vector<uint8_t>> rslt;
  auto found = resources.find(pathfn);
  if (found != resources.end()) {
    auto rs = found->second;
    // Cycle through sources until we get the file, starting from the last source loaded
    while ((rs.size() > 0) && (!rslt || (rslt->size() == 0))) {
      auto &r = rs[rs.size() - 1];
      if (r.storage == STORAGE::LOCAL_DIR) {
        rslt = read_file_local_dir(r);
      } else {
        rslt = read_file_grf(r);
      }
      rs.pop_back();
    }
  }
  return rslt;
}

std::optional<std::string> ares::grf::resource_set::find_resource_file(const std::string& src) {
  auto found = resnametable.find(src);
  if (found != resnametable.end()) {
    return std::optional<std::string>("data\\" + found->second);
  } else {
    return std::optional<std::string>();
  }
}

std::optional<std::vector<uint8_t>> ares::grf::resource_set::read_file_local_dir(const resource& r) {
  std::optional<std::vector<uint8_t>> rslt;
  std::ifstream f(*r.source + "/" + r.name);
  rslt->resize(r.uncompressed_sz);
  f.read((char*)rslt->data(), rslt->size());
  return rslt;
}

static void * AllocForLzma(ISzAllocPtr, size_t size) { return malloc(size); }
static void FreeForLzma(ISzAllocPtr, void *address) { free(address); }
static ISzAlloc SzAllocForLzma = { &AllocForLzma, &FreeForLzma };

std::optional<std::vector<uint8_t>> ares::grf::resource_set::read_file_grf(const resource& r) {
  std::optional<std::vector<uint8_t>> rslt;
  std::vector<uint8_t> compressed;
  std::vector<uint8_t> uncompressed;
  compressed.resize(r.compressed_sz_aligned);
  uncompressed.resize(r.uncompressed_sz);
  std::ifstream f(*r.source);
  f.seekg(r.offset);
  f.read((char*)compressed.data(), compressed.size());
  decode_grf(compressed, FILELIST_TYPE(r.type), r.compressed_sz);

  if (compressed[0] == 0) {
    SizeT actual_sz = uncompressed.size();
    ELzmaStatus lzma_status;
    auto data_sz = compressed.size() - LZMA_PROPS_SIZE - 1;
    auto lzma_res = LzmaDecode(uncompressed.data(), &actual_sz,
                               &compressed[1 + LZMA_PROPS_SIZE], &data_sz,
                               &compressed[1], LZMA_PROPS_SIZE,
                               LZMA_FINISH_ANY, &lzma_status, &SzAllocForLzma);
    if ((lzma_res != SZ_OK) || (actual_sz != r.uncompressed_sz)) {
      throw std::runtime_error("ares::grf could not uncompress LZMA-compressed resource '" + r.name + "' from source '" + *r.source +
                               "', lzma code " + std::to_string(lzma_res) +
                               ", compressed size " + std::to_string(compressed.size()) +
                               ", expected uncompressed size " + std::to_string(r.uncompressed_sz) +
                               ", actual " + std::to_string(actual_sz));
    }
  } else {
    auto actual_sz = uLongf(uncompressed.size());
    auto zlib_rc = uncompress((Bytef*)uncompressed.data(), &actual_sz, (const Bytef*)compressed.data(), uLong(compressed.size()));
    if ((zlib_rc != Z_OK) || (actual_sz != r.uncompressed_sz)) {
      throw std::runtime_error("ares::grf could not uncompress Zlib-compressed resource '" + r.name + "' from source '" + *r.source +
                               "', zlib code " + std::to_string(zlib_rc) +
                               ", compressed size " + std::to_string(compressed.size()) +
                               ", expected uncompressed size " + std::to_string(r.uncompressed_sz) +
                               ", actual " + std::to_string(actual_sz));
    }
  }
  
  rslt.emplace(std::move(uncompressed));
  return rslt;
}

void ares::grf::resource_set::decode_grf(std::vector<uint8_t>& compressed, FILELIST_TYPE type, uint32_t compressed_sz) const {
  auto p = (uint64_t*)compressed.data();
  const size_t nblocks = compressed_sz / sizeof(uint64_t);
  if ((type & uint8_t(FILELIST_TYPE::ENCRYPT_HEADER)) ||
      (type & uint8_t(FILELIST_TYPE::ENCRYPT_MIXED))) {
    for (size_t i = 0; (i < 20) && (i < nblocks); ++i) des::decrypt_block(p[i]);    
  }
  if (type & uint8_t(FILELIST_TYPE::ENCRYPT_MIXED)) {
    size_t digits{1};
    for (size_t i = 10; i <= compressed_sz; i *= 10) ++digits;
    // choose size of gap between two encrypted blocks
    // digits:  0  1  2  3  4  5  6  7  8  9 ...
    //  cycle:  1  1  1  4  5 14 15 22 23 24 ...
    const size_t cycle = ( digits < 3 ) ? 1
      : ( digits < 5 ) ? digits + 1
      : ( digits < 7 ) ? digits + 9
      :                  digits + 15;
    int j{-1};
    for (size_t i = 0; i < nblocks; ++i) {
      if (i % cycle == 0) {
        des::decrypt_block(p[i]);
        continue;
      }
      ++j;
      if ((j % 7 == 0) && (j != 0)) {
        decrypt_grf_shuffle(p[i]);
        continue;
      }
    }
  }
}

void ares::grf::resource_set::decrypt_grf_shuffle(uint64_t& src) const {
  uint64_t tmp{0};
  auto s = (uint8_t*)src;
  auto d = (uint8_t*)&tmp;
  d[0] = s[3];
  d[1] = s[4];
  d[2] = s[6];
  d[3] = s[0];
  d[4] = s[1];
  d[5] = s[2];

  switch (s[7]) {
  case 0x00: d[7] = 0x2B; break;
  case 0x2B: d[7] = 0x00; break;
  case 0x6C: d[7] = 0x80; break;
  case 0x01: d[7] = 0x68; break;
  case 0x68: d[7] = 0x01; break;
  case 0x48: d[7] = 0x77; break;
  case 0x60: d[7] = 0xFF; break;
  case 0x77: d[7] = 0x48; break;
  case 0xB9: d[7] = 0xC0; break;
  case 0xC0: d[7] = 0xB9; break;
  case 0xFE: d[7] = 0xEB; break;
  case 0xEB: d[7] = 0xFE; break;
  case 0x80: d[7] = 0x6C; break;
  case 0xFF: d[7] = 0x60; break;
  default: d[7] = s[7]; break;
  }
  
  src = tmp;
}

void ares::grf::resource_set::parse_resnametable(const std::string& buf) {
  namespace fs = std::experimental::filesystem;
  const char* b = buf.c_str();
  size_t start = 0;
  size_t end = buf.find("\r\n");
  if (end == std::string::npos) end = buf.size(); else end += 2;
  while ((end - start > 0) && (start != std::string::npos)) {
    if (!((buf[start] == '/') && (buf[start+1] == '/'))) {
      std::string_view row(&b[start], end - start);
      auto split1 = row.find('#');
      auto split2 = row.find('#', split1 + 1);
      if ((split1 != std::string::npos) && (split2 != std::string::npos)) {
        std::string src(std::string_view(&row[0], split1));
        std::string dst(std::string_view(&row[split1 + 1], split2 - split1 - 1));
        if ((fs::path(dst).extension() == ".gat") || (fs::path(dst).extension() == ".rsw")) {
          resnametable[src] = dst;
        }
      }
    }
    start = end;
    end = buf.find("\r\n", end);
    if (end == std::string::npos) { end = buf.size(); } else end += 2;
  }
}
