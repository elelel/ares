#include "../accounts.hpp"

ares::database::accounts::num_chars::num_chars(result_type& rslt, const uint32_t& id, const size_t& max_chars) :
  rslt(rslt),
  id_(id),
  max_chars_(max_chars) {
  }

void ares::database::accounts::num_chars::operator()(argument_type& trans) {
  auto qr = trans.prepared("num_chars_for_aid")(id_)(max_chars_).exec();
  if (qr.size() == 1) {
    size_t cnt;
    qr[0]["cnt"].to(cnt);
    rslt.emplace(cnt);
  }
}
