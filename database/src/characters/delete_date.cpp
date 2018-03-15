#include "../characters.hpp"

ares::database::characters::delete_date::delete_date(result_type& rslt, const model::character_id& character_id) :
  rslt(rslt),
  character_id_(character_id) {
}

void ares::database::characters::delete_date::operator()(argument_type& trans) {
  auto qr = trans.prepared("character_delete_date")(character_id_).exec();
  if (qr.size() == 1) {
    qr[0]["delete_date"].to(rslt);
  }
} 
