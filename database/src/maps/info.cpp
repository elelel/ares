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
    model::map_info r;
    qr[0]["x_size"].to(r.x_size);
    qr[0]["y_size"].to(r.y_size);
    pqxx::binarystring blob(qr[0]["cell_flags"]);
    size_t actual_sz = r.x_size * r.y_size;
    size_t data_sz = blob.size();
    std::vector<uint8_t> uncompressed(actual_sz);
    ELzmaStatus lzma_status;
    auto lzma_res = LzmaDecode(uncompressed.data(), &actual_sz,
                               &blob[LZMA_PROPS_SIZE], &data_sz,
                               &blob[0], LZMA_PROPS_SIZE,
                               LZMA_FINISH_END, &lzma_status, &SzAllocForLzma);
    if ((lzma_res == SZ_OK) && (actual_sz == size_t(r.x_size * r.y_size))) {
      for (const auto& f : uncompressed)
        r.cell_flags.push_back(model::map_cell_flags::from_uint8(f));
      rslt.emplace(std::move(r));
    }
  }
}
