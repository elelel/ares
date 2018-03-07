#include "../maps.hpp"

ares::database::maps::ids_and_names::ids_and_names(result_type& rslt) :
  rslt(rslt) {
}

void ares::database::maps::ids_and_names::operator()(argument_type& trans) {
  rslt.clear();
  auto qr = trans.prepared("map_ids_and_names").exec();
  for (const auto& row : qr) {
    std::tuple<uint32_t, std::string> r;
    row["id"].to(std::get<0>(r));
    row["name"].to(std::get<1>(r));
    rslt.push_back(std::move(r));
  }
}
