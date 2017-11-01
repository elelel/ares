#include "database.hpp"

namespace ares {
  namespace account {
    namespace db {
      struct create_user : pqxx::transactor<> {
        create_user(const std::string& login, const std::string& password) :
          transactor<>("create_user"),
          login_(login),
          password_(password) {
        }

        void operator()(argument_type& trans) {
          trans.conn().prepare("create_user_query", R"(
INSERT INTO "users" (login, password) VALUES ($1, crypt($2, gen_salt('bf')));
)");
          trans.prepared("create_user_query")(login_)(password_).exec();
        }
      private:
        const std::string& login_;
        const std::string& password_;
      };
    }
  }
}

void ares::account::database::create_user(const std::string& login, const std::string& password) {
  with_wait_lock([this, &login, &password] () {
    db::create_user t(login, password);
    pqxx_conn_->perform(t);
    });
}

