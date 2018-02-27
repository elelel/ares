#include "../database.hpp"

namespace ares {
  namespace database {
    namespace detail {
      struct create_user : pqxx::transactor<> {
        create_user(const std::string& login, const std::string& password, const std::string& email, const uint8_t level) :
          login_(login),
          password_(password),
          email_(email),
          level_(level) {
        }

        void operator()(argument_type& trans) {
          trans.prepared("create_user")(login_)(password_)(email_)(level_).exec();
        }
      private:
        const std::string& login_;
        const std::string& password_;
        const std::string& email_;
        const uint8_t level_;
      };
    }
  }
}

void ares::database::db::create_user(const std::string& login, const std::string& password, const std::string& email, const uint8_t level) {
  with_wait_lock([this, &login, &password, &email, level] () {
      detail::create_user t(login, password, email, level);
      pqxx_conn_->perform(t);
    });
}

