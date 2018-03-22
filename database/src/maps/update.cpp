#include "../maps.hpp"

#include <LzmaEnc.h>

static void * AllocForLzma(ISzAllocPtr, size_t size) { return malloc(size); }
static void FreeForLzma(ISzAllocPtr, void *address) { free(address); }
static ISzAlloc SzAllocForLzma = { &AllocForLzma, &FreeForLzma };
SRes OnProgress(const ICompressProgress*, UInt64, UInt64) { return SZ_OK; }
static ICompressProgress g_ProgressCallback = { &OnProgress };

ares::database::maps::update::update(result_type& rslt, const std::string& name, const model::map_info& info) :
  rslt(rslt),
  name_(name),
  info_(info) {
  }

void ares::database::maps::update::operator()(argument_type& trans) {
  std::vector<uint8_t> compressed(info_.x_size() * info_.y_size());
  size_t compressed_sz(compressed.size() - LZMA_PROPS_SIZE);
  size_t props_sz = LZMA_PROPS_SIZE;
  CLzmaEncProps props;
  LzmaEncProps_Init(&props);
  props.writeEndMark = 1;
  auto lzma_res = LzmaEncode(&compressed[LZMA_PROPS_SIZE], &compressed_sz,
                             (const Byte*)info_.static_flags().data(), info_.static_flags().size(),
                             &props, compressed.data(), &props_sz, props.writeEndMark,
                             &g_ProgressCallback, &SzAllocForLzma, &SzAllocForLzma);
  if ((lzma_res == SZ_OK) && (props_sz == LZMA_PROPS_SIZE)) {
    pqxx::binarystring blob(compressed.data(), props_sz + compressed_sz);

    auto get_id = trans.prepared("map_id_by_name")(name_).exec();
    if (get_id.size() == 1) {
      uint32_t map_id;
      get_id[0]["id"].to(map_id);
      trans.prepared("map_update")(map_id)(info_.x_size())(info_.y_size())(blob).exec();
      rslt.emplace(map_id);
      return;
    } else if (get_id.size() == 0) {
      trans.prepared("map_insert")(name_)(info_.x_size())(info_.y_size())(blob).exec();
      auto qr = trans.prepared("map_id_by_name")(name_).exec();
      if (qr.size() == 1) {
        uint32_t map_id;
        qr[0]["id"].to(map_id);
        rslt.emplace(map_id);
        return;
      }
    }
  }
  trans.abort();
}

