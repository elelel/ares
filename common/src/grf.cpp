#include "grf.hpp"

#include <algorithm>
#include <experimental/filesystem>
#include <system_error>
#include <sstream>
#include <fstream>
#include <iostream>
#include <type_traits>

#include "des.hpp"
#include "grf_structs.hpp"

ares::grf::resource_set::resource_set(const std::vector<std::string>& filenames, const size_t resnametable_idx) :
  resnametable_idx_(resnametable_idx) {
  size_t i{0};
  for (const auto& filename : filenames) {
    std::error_code ec;
    namespace fs = std::experimental::filesystem;
    auto filepath = fs::path(filename);
    if (fs::exists(filepath, ec) && (ec.value() == 0)) {
      if (fs::is_directory(filepath)) {
        add_local_dir(filepath, i);
      } else if (fs::is_regular_file(filepath) && (filepath.extension() == ".grf")) {
        add_grf(filepath, i);
      }
    } else {
      throw std::runtime_error("ares::grf path " + filename + " does not exist or error determining file existence");
    }
    ++i;
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

void ares::grf::resource_set::add_local_dir(const std::string& dir_path, const size_t source_idx) {
  namespace fs = std::experimental::filesystem;
  for (const auto& p : fs::recursive_directory_iterator(dir_path)) {
    if (fs::is_regular_file(p)) {
      std::string path = p.path();
      resource f;
      f.storage = STORAGE::LOCAL_DIR;
      f.source = get_source(dir_path);
      f.name = path;
      f.uncompressed_sz = fs::file_size(p.path());

      if ((source_idx == resnametable_idx_ ) && (f.name == "resnametable.txt")) {
        std::ifstream file(path, std::ios::binary | std::ios::ate);
        std::streamsize sz = file.tellg();
        std::string text(sz, '\0');
        file.read(text.data(), sz);
        parse_resnametable(text);
      }
      resources[path] = std::move(f);
    }
  }
}

void ares::grf::resource_set::add_grf(const std::string& grf_path, const size_t source_idx) {
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
          uLongf actual_sz = uncompressed.size();
          uncompress((Bytef*)uncompressed.data(), &actual_sz, (const Bytef*)compressed.data(), compressed.size());
          if (actual_sz == uncompressed.size()) {
            size_t ofs = 0;
            std::cout << " grf index " << source_idx << " " << resnametable_idx_ << std::endl;
            while (ofs < uncompressed.size()) {
              resource r;
              r.storage = STORAGE::GRF;
              r.source = get_source(grf_path);
              r.name = (char*)&uncompressed[ofs];
              auto info = (structs::filetable_entry_info*)&uncompressed[ofs + r.name.size() + 1];
              r.compressed_sz = info->compressed_sz;
              r.compressed_sz_aligned = info->compressed_sz_aligned;
              r.uncompressed_sz = info->uncompressed_sz;
              r.offset = info->offset + sizeof(grf_header);
              r.type = info->type;
              ofs += r.name.size() + 1 + sizeof(structs::filetable_entry_info);
              auto fname = r.name;
              resources[fname] = std::move(r);
              //              std::cout << fname << std::endl;
              if ((source_idx == resnametable_idx_ ) && (fname == "data\\resnametable.txt")) {
                auto buf = read_file(fname);
                if (buf) {
                  std::string text(buf->begin(), buf->end());
                  parse_resnametable(text);
                } else {
                  throw std::runtime_error("ares::grf resnametable was not found in " + grf_path);
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
  auto found = resources.find(pathfn);
  if (found != resources.end()) {
    if (found->second.storage == STORAGE::LOCAL_DIR) {
      return read_file_local_dir(found->second);
    } else {
      return read_file_grf(found->second);
    }
  } else {
    std::cout << "File not found " << pathfn << std::endl;
    return std::optional<std::vector<uint8_t>>();
  }
}

std::optional<std::vector<uint8_t>> ares::grf::resource_set::read_file_local_dir(const resource& r) {
  std::optional<std::vector<uint8_t>> rslt;
  std::ifstream f(*r.source + "/" + r.name);
  rslt->resize(r.uncompressed_sz);
  f.read((char*)rslt->data(), rslt->size());
  return rslt;
}

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
  uLongf actual_sz = uncompressed.size();
  uncompress((Bytef*)uncompressed.data(), &actual_sz, (const Bytef*)compressed.data(), compressed.size());
  if (actual_sz != r.uncompressed_sz) {
    throw std::runtime_error("ares::grf uncompressed resource size mismatch");
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

void ares::grf::resource_set::load_resnametable(const std::string& from_path) {
  namespace fs = std::experimental::filesystem;
  std::error_code ec;
  if (fs::exists(from_path, ec) && (ec.value() == 0)) {
    if (fs::is_directory(from_path)) {
    } else if (fs::is_regular_file(from_path) && (fs::path(from_path).extension() == ".grf")) {
      auto buf = read_file(from_path);
      if (buf) {
        std::cout << "Read resname table " << std::endl;
      } else {
        throw std::runtime_error("ares::grf " + from_path + " could not be read from the specified grf");
      }
    } else {
      throw std::runtime_error("ares::grf " + from_path + " is invalid location for resnametable (neither a directory, nor a regular file with 'grf' extension");
    }
  } else {
    throw std::runtime_error("ares::grf resnametable path " + from_path + " does not exist or error determining file existence");
  }
}

void ares::grf::resource_set::parse_resnametable(const std::string& buf) {
  namespace fs = std::experimental::filesystem;
  resnametable.clear();
  std::stringstream s(buf);
  while (s.good() && !s.eof()) {
    char line[1024];
    s.getline(line, 1024);
    char w1[256];
    char w2[256];
    // TODO: Fix line parsing
    if (sscanf(line, "%255[^#\r\n]#%255[^#\r\n]#", w1, w2) == 2 ) {
      auto src = fs::path(w1);
      auto dst = fs::path(w2);
      if ((dst.extension() == "gat") || (dst.extension() == "rsw")) {
        resnametable[src] = dst;
      }
    }
  }
}

