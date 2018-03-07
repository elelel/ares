#include "../accounts.hpp"

ares::database::accounts::exists::exists(result_type& rslt, const std::string& login) :
  rslt(rslt),
  login_(login) {
}

void ares::database::accounts::exists::operator()(argument_type& trans) {
  auto qr = trans.prepared("account_exists")(login_).exec();
  if (qr.size() > 0) rslt = true; else rslt = false;
}

