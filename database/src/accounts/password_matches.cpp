#include "../accounts.hpp"

ares::database::accounts::password_matches::password_matches(result_type& rslt, const std::string& login, const std::string& password) :
  rslt(rslt),
  login_(login),
  password_(password) {
  }

void ares::database::accounts::password_matches::operator()(argument_type& trans) {
  auto qr = trans.prepared("account_password_matches")(login_)(password_).exec();
  if (qr.size() > 0) rslt = true; else rslt = false;
}
