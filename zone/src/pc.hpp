#pragma once

#include <memory>

#include <ares/model>

namespace ares {
  namespace zone {

    struct map;
    
    struct pc {
      pc(const model::pc_info& info, std::shared_ptr<zone::map> map);
    private:
      model::pc_info info_;
      std::weak_ptr<zone::map> map_;
    };
  }
}
