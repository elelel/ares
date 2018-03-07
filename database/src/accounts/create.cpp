#include "../accounts.hpp"

ares::database::accounts::create::create(const std::string& login, const std::string& password, const std::string& email, const uint8_t level) :
  login_(login),
  password_(password),
  email_(email),
  level_(level) {
  }

void ares::database::accounts::create::operator()(argument_type& trans) {
  trans.prepared("account_create")(login_)(password_)(email_)(level_).exec();
}
