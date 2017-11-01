#include "database.hpp"

namespace ares {
  namespace account {
    namespace db {
      struct password_matches : pqxx::transactor<> {
        password_matches(const std::string& login, const std::string& password, bool& rslt) :
          login_(login),
          password_(password),
          rslt_(rslt) {
        }

        void operator()(argument_type& trans) {
          trans.conn().prepare("password_matches_query", R"(
SELECT "id" FROM "users"
    WHERE ("login" = $1) AND (password = crypt($2, password)) LIMIT 1;
)");
          auto rslt = trans.prepared("password_matches_query")(login_)(password_).exec();
          if (rslt.size() > 0) rslt_ = true; else rslt_ = false;
        }
      private:
        const std::string& login_;
        const std::string& password_;
        bool& rslt_;
      };
    }
  }
}

bool ares::account::database::password_matches(const std::string& login, const std::string& password) {
  bool rslt{false};
  with_wait_lock([this, &login, &password, &rslt] () {
      db::password_matches t(login, password, rslt);
      pqxx_conn_->perform(t);
    });
  return rslt;
}
