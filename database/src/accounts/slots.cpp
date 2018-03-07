#include "../accounts.hpp"

ares::database::accounts::slots::slots(result_type& rslt, const uint32_t id) :
  rslt(rslt),
  id_(id) {
  }

void ares::database::accounts::slots::operator()(argument_type& trans) {
  auto qr = trans.prepared("account_slots_for_id")(id_).exec();
  if (qr.size() == 1) {
    record::account_slots r;
    qr[0]["normal_slots"].to(r.normal_slots);
    qr[0]["premium_slots"].to(r.premium_slots);
    qr[0]["billing_slots"].to(r.billing_slots);
    qr[0]["creatable_slots"].to(r.creatable_slots);
    qr[0]["playable_slots"].to(r.playable_slots);
    rslt.emplace(std::move(r));
  }
}
