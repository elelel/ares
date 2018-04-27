#include "pcs_manager.hpp"

#include "map/map.hpp"

ares::zone::pcs_manager::pcs_manager(std::shared_ptr<spdlog::logger> log) :
  log_(log) {
}

void ares::zone::pcs_manager::add(const model::character_id& char_id, std::shared_ptr<zone::pc> pc, std::shared_ptr<zone::map> map, std::shared_ptr<session> s) {
  auto found = pcs_.find(pc);
  if (found == pcs_.end()) {
    pcs_.insert(pc);
    id_to_session_.insert({char_id, s});
    map->add_pc(pc);
  } else {
    log_->error("Can't add char id {} to pcs manager: pc structure already exists", char_id.to_string());
  }
}
