#pragma once

#include <map>
#include <memory>

#include <ares/model>

namespace ares {
  namespace zone {

    struct pc;
    
    struct map {
      map(const uint32_t id, const model::map_info& info);

    private:
      std::map<uint32_t, std::vector<std::weak_ptr<zone::pc>>> pcs_;
      //      std::map<uint32_t, std::vector<std::weak_ptr<...>>> mobs_;

      uint32_t id_;
      model::map_info info_;
    };
  }
}