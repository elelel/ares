#include "../database.hpp"

namespace ares {
  namespace database {
    namespace detail {
      struct password_matches : pqxx::transactor<> {
        password_matches(const std::string& login, const std::string& password, bool& rslt) :
          login_(login),
          password_(password),
          rslt_(rslt) {
        }

        void operator()(argument_type& trans) {
          auto rslt = trans.prepared("password_matches")(login_)(password_).exec();
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

bool ares::database::db::password_matches(const std::string& login, const std::string& password) {
  bool rslt{false};
  with_wait_lock([this, &login, &password, &rslt] () {
      detail::password_matches t(login, password, rslt);
      pqxx_conn_->perform(t);
    });
  return rslt;
}
