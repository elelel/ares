#include "../accounts.hpp"

ares::database::accounts::storage::storage(result_type& rslt, const uint32_t aid) :
  rslt(rslt),
  id_(aid) {
}

void ares::database::accounts::storage::operator()(argument_type& trans) {
  auto qr = trans.prepared("account_storage_for_aid")(id_).exec();
  if (qr.size() == 1) {
    record::account_storage r;
    qr[0]["bank_vault"].to(r.bank_vault);
    qr[0]["max_storage"].to(r.max_storage);
    rslt.emplace(std::move(r));
  }
}
