#pragma once

#include <chrono>
#include <memory>

#include <ares/model>

namespace ares {
  namespace zone {

    struct session;
    struct map;
    
    struct pc {
      pc(const model::pc_info& info, std::shared_ptr<zone::map> map, std::shared_ptr<zone::session> sess);

      std::chrono::milliseconds speed();
      
      const std::weak_ptr<zone::session>& session() const;

      std::weak_ptr<zone::map> map;
      model::pc_info info;
    private:

      std::weak_ptr<zone::session> session_;
    };
  }
}
