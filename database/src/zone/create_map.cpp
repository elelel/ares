#include "../database.hpp"

#include <LzmaEnc.h>

static void * AllocForLzma(ISzAllocPtr, size_t size) { return malloc(size); }
static void FreeForLzma(ISzAllocPtr, void *address) { free(address); }
static ISzAlloc SzAllocForLzma = { &AllocForLzma, &FreeForLzma };
SRes OnProgress(const ICompressProgress*, UInt64, UInt64) { return SZ_OK; }
static ICompressProgress g_ProgressCallback = { &OnProgress };

namespace ares {
  namespace database {
    namespace detail {
      struct recreate_map : pqxx::transactor<> {
        recreate_map(const std::string& name, const model::map_info& info, std::optional<uint32_t>& rslt) :
          name_(name),
          info_(info),
          rslt_(rslt) {
        }

        void operator()(argument_type& trans) {
          std::vector<uint8_t> compressed(info_.x_size * info_.y_size);
          size_t compressed_sz(compressed.size() - LZMA_PROPS_SIZE);
          size_t props_sz = LZMA_PROPS_SIZE;
          CLzmaEncProps props;
          LzmaEncProps_Init(&props);
          props.writeEndMark = 1;
          auto lzma_res = LzmaEncode(&compressed[LZMA_PROPS_SIZE], &compressed_sz,
                                     (const Byte*)info_.cell_flags.data(), info_.cell_flags.size(),
                                     &props, compressed.data(), &props_sz, props.writeEndMark,
                                     &g_ProgressCallback, &SzAllocForLzma, &SzAllocForLzma);
          if ((lzma_res == SZ_OK) && (props_sz == LZMA_PROPS_SIZE)) {
            pqxx::binarystring blob(compressed.data(), props_sz + compressed_sz);

            auto get_id = trans.prepared("map_id_by_name")(name_).exec();
            if (get_id.size() == 1) {
              uint32_t map_id;
              get_id[0]["id"].to(map_id);
              trans.prepared("update_map_info")(map_id)(info_.x_size)(info_.y_size)(blob).exec();
              rslt_.emplace(map_id);
              return;
            } else if (get_id.size() == 0) {
              trans.prepared("insert_map_info")(name_)(info_.x_size)(info_.y_size)(blob).exec();
              auto qr = trans.prepared("map_id_by_name")(name_).exec();
              if (qr.size() == 1) {
                uint32_t map_id;
                qr[0]["id"].to(map_id);
                rslt_.emplace(map_id);
                return;
              }
            }
          }
          trans.abort();
        }

      private:
        const std::string& name_;
        const model::map_info& info_;
        std::optional<uint32_t>& rslt_;
      };
    }
  }
}

auto ares::database::db::recreate_map(const std::string& map_name, const model::map_info& map_info) -> std::optional<uint32_t> {
  SPDLOG_TRACE(log_, "database::recreate_map map_name: '{}'", map_name);
  std::optional<uint32_t> rslt;
  with_wait_lock([this, &map_name, &map_info, &rslt] () {
      detail::recreate_map t(map_name, map_info, rslt);
      pqxx_conn_->perform(t);
    });
  return rslt;
}
