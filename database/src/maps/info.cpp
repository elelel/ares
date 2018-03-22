#include "../maps.hpp"

#include <LzmaDec.h>

static void * AllocForLzma(ISzAllocPtr, size_t size) { return malloc(size); }
static void FreeForLzma(ISzAllocPtr, void *address) { free(address); }
static ISzAlloc SzAllocForLzma = { &AllocForLzma, &FreeForLzma };

ares::database::maps::info::info(result_type& rslt, const uint32_t& id) :
  rslt(rslt),
  id_(id) {
  }

void ares::database::maps::info::operator()(argument_type& trans) {
  auto qr = trans.prepared("map_by_id")(id_).exec();
  if (qr.size() == 1) {
    uint16_t x_size;
    uint16_t y_size;
    qr[0]["x_size"].to(x_size);
    qr[0]["y_size"].to(y_size);
    model::map_info i(x_size, y_size);
    pqxx::binarystring blob(qr[0]["cell_flags"]);

    size_t data_sz = blob.size();

    auto& uncompressed = i.static_flags();
    size_t actual_sz = uncompressed.size();
    
    ELzmaStatus lzma_status;
    auto lzma_res = LzmaDecode((Byte*)uncompressed.data(), &actual_sz,
                               &blob[LZMA_PROPS_SIZE], &data_sz,
                               &blob[0], LZMA_PROPS_SIZE,
                               LZMA_FINISH_END, &lzma_status, &SzAllocForLzma);
    if ((lzma_res == SZ_OK) && (actual_sz == uncompressed.size())) {
      rslt.emplace(std::move(i));
    }
  }
}
