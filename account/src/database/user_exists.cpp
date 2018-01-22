#include "database.hpp"

namespace ares {
  namespace account {
    namespace db {
      struct user_exists : pqxx::transactor<> {
        user_exists(const std::string& login, bool& rslt) :
          transactor<>("user_exists"),
          login_(login), 
          rslt_(rslt) {
        }

        void operator()(argument_type& trans) {
          auto result = trans.prepared("user_exists")(login_).exec();
          if (result.size() > 0) rslt_ = true; else rslt_ = false;
        }
      private:
        const std::string& login_;
        bool& rslt_;
      };
    }
  }
}

bool ares::account::database::user_exists(const std::string& login) {
  bool rslt{false};
  with_wait_lock([this, &login, &rslt]() {
      db::user_exists t(login, rslt);
      pqxx_conn_->perform(t);
    });
  return rslt;
}
