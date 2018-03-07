#include "../characters.hpp"

ares::database::characters::delete_date::delete_date(result_type& rslt, const uint32_t& id) :
  rslt(rslt),
  id_(id) {
}

void ares::database::characters::delete_date::operator()(argument_type& trans) {
  auto qr = trans.prepared("char_delete_date")(id_).exec();
  if (qr.size() == 1) {
    qr[0]["delete_date"].to(rslt);
  }
} 
