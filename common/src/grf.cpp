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

ares::grf::resource_set::resource_set(const std::vector<std::string>& filenames, const size_t resnametable_idx) {
  for (const auto& filename : filenames) {
    std::error_code ec;
    namespace fs = std::experimental::filesystem;
    auto filepath = fs::path(filename);
    if (fs::exists(filepath, ec) && (ec.value() == 0)) {
      if (fs::is_directory(filepath)) {
        add_local_dir(filepath);
      } else if (fs::is_regular_file(filepath) && (filepath.extension() == ".grf")) {
        add_grf(filepath);
      }
    } else {
      throw std::runtime_error("ares::grf path " + filename + " does not exist or error determining file existence");
    }
  }
  load_resnametable(filenames.at(resnametable_idx));  
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
      std::string path = p.path();
      resource f;
      f.storage = STORAGE::LOCAL_DIR;
      f.source = get_source(dir_path);
      f.name = path;
      f.uncompressed_sz = fs::file_size(p.path());
      resources[path] = std::move(f);
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
          uLongf actual_sz = uncompressed.size();
          uncompress((Bytef*)uncompressed.data(), &actual_sz, (const Bytef*)compressed.data(), compressed.size());
          if (actual_sz == uncompressed.size()) {
            size_t ofs = 0;
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

void ares::grf::resource_set::load_resnametable(const std::string& from_path) {
  namespace fs = std::experimental::filesystem;
  std::error_code ec;
  if (fs::exists(from_path, ec) && (ec.value() == 0)) {
    if (fs::is_directory(from_path)) {
      auto file_path = from_path + "/resnametable.txt";
      std::ifstream file(file_path, std::ios::binary | std::ios::ate);
      std::streamsize sz = file.tellg();
      std::string text(sz, '\0');
      file.read(text.data(), sz);
      parse_resnametable(text);
    } else if (fs::is_regular_file(from_path) && (fs::path(from_path).extension() == ".grf")) {
      
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
    if (sscanf(line, "%255[^#\r\n]#%255[^#\r\n]#", w1, w2) == 2 ) {
      auto src = fs::path(w1);
      auto dst = fs::path(w2);
      if ((dst.extension() == "gat") || (dst.extension() == "rsw")) {
        resnametable[src] = dst;
      }
    }
  }
}

