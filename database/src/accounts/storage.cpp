#include "../accounts.hpp"

ares::database::accounts::storage::storage(result_type& rslt, const model::account_id account_id) :
  rslt(rslt),
  account_id_(account_id) {
}

void ares::database::accounts::storage::operator()(argument_type& trans) {
  auto qr = trans.prepared("account_storage_for_account_id")(account_id_).exec();
  if (qr.size() == 1) {
    record::account_storage r;
    qr[0]["bank_vault"].to(r.bank_vault);
    qr[0]["max_storage"].to(r.max_storage);
    rslt.emplace(std::move(r));
  }
}
