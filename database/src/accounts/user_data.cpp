#include "../accounts.hpp"

template <typename QueryResult>
inline void assign_result(std::optional<ares::database::accounts::record::user_data>& rslt, QueryResult& qr) {
  if (qr.size() > 0) {
    ares::database::accounts::record::user_data r;
    qr[0]["id"].to(r.account_id);
    qr[0]["login"].to(r.login);
    qr[0]["email"].to(r.email);
    qr[0]["level"].to(r.level);
    qr[0]["sex"].to(r.sex);
    if (!qr[0]["expiration_time"].to(r.expiration_time)) r.expiration_time.reset();
    if (!qr[0]["birthdate"].to(r.birthdate)) r.birthdate.reset();
    if (!qr[0]["pin"].is_null()) {
      r.pin.emplace(qr[0]["pin"].c_str());
    }
    rslt.emplace(std::move(r));
  } else {
    rslt.reset();
  }
}

ares::database::accounts::user_data_for_login::user_data_for_login(result_type& rslt, const std::string& login) :
  rslt(rslt),
  login_(login) {
}

void ares::database::accounts::user_data_for_login::operator()(argument_type& trans) {
  auto qr = trans.prepared("user_data_for_login")(login_).exec();
  assign_result(rslt, qr);
}


ares::database::accounts::user_data_for_id::user_data_for_id(result_type& rslt, const model::account_id& account_id) :
  rslt(rslt),
  account_id_(account_id) {
}

void ares::database::accounts::user_data_for_id::operator()(argument_type& trans) {
  auto qr = trans.prepared("user_data_for_account_id")(account_id_).exec();
  assign_result(rslt, qr);
}
