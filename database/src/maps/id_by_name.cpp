#include "../maps.hpp"

ares::database::maps::id_by_name::id_by_name(result_type& rslt, const std::string& name) :
  rslt(rslt),
  name_(name) {
}

void ares::database::maps::id_by_name::operator()(argument_type& trans) {
  auto qr = trans.prepared("map_id_by_name")(name_).exec();
  if (qr.size() == 1) {
    uint32_t id;
    qr[0]["id"].to(id);
    rslt.emplace(id);
  }
}
