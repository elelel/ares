#include "pc.hpp"

ares::zone::pc::pc(const model::pc_info& info, std::shared_ptr<zone::map> map) :
  info_(info), map_(map) {
}
