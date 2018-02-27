#include "../database.hpp"

namespace ares {
  namespace database {
    namespace detail {
      struct create_user : pqxx::transactor<> {
        create_user(const std::string& login, const std::string& password) :
          login_(login),
          password_(password) {
        }

        void operator()(argument_type& trans) {
          trans.prepared("create_user")(login_)(password_).exec();
        }
      private:
        const std::string& login_;
        const std::string& password_;
      };
    }
  }
}

void ares::database::db::create_user(const std::string& login, const std::string& password) {
  with_wait_lock([this, &login, &password] () {
      detail::create_user t(login, password);
      pqxx_conn_->perform(t);
    });
}

