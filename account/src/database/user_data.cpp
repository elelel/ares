#include "database.hpp"

namespace ares {
  namespace account {
    namespace db {
      template <typename QueryResult>
      void assign_result(std::optional<record::user_data>& rslt, QueryResult& qr) {
        if (qr.size() > 0) {
          record::user_data r;
          qr[0]["id"].to(r.aid);
          qr[0]["login"].to(r.login);
          qr[0]["email"].to(r.email);
          qr[0]["level"].to(r.level);
          int sex;
          qr[0]["sex"].to(sex);
          r.sex = uint8_t(sex);
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
      
      struct user_data_for_login : pqxx::transactor<> {
        user_data_for_login(const std::string& login, std::optional<record::user_data>& rslt) :
          login_(login),
          rslt_(rslt) {
        }

        void operator()(argument_type& trans) {
          trans.conn().prepare("user_data_for_login_query", R"(
SELECT "id", "login", "email", "level", "sex", "expiration_time", "birthdate", "pin"  FROM "users"
    WHERE ("login" = $1) LIMIT 1;
)");
          auto qr = trans.prepared("user_data_for_login_query")(login_).exec();
          assign_result(rslt_, qr);
        }
      private:
        const std::string& login_;
        std::optional<record::user_data>& rslt_;
      };

      struct user_data_for_aid : pqxx::transactor<> {
        user_data_for_aid(const size_t aid, std::optional<record::user_data>& rslt) :
          aid_(aid),
          rslt_(rslt) {
        }

        void operator()(argument_type& trans) {
          trans.conn().prepare("user_data_for_aid_query", R"(
SELECT "id", "login", "level", "email", "sex", "expiration_time", "birthdate", "pin"  FROM "users"
  WHERE ("id" = $1) LIMIT 1;
)");
          auto qr = trans.prepared("user_data_for_aid_query")(aid_).exec();
          assign_result(rslt_, qr);
        }
      private:
        const uint32_t aid_;
        std::optional<record::user_data>& rslt_;
      };
    }
  }
}

auto ares::account::database::user_data_for_login(const std::string& login) -> std::optional<db::record::user_data> {
  std::optional<db::record::user_data> rslt;
  with_wait_lock([this, &login, &rslt] () {
      db::user_data_for_login t(login, rslt);
      pqxx_conn_->perform(t);
    });
  return rslt;
}

auto ares::account::database::user_data_for_aid(const uint32_t aid) -> std::optional<db::record::user_data> {
  std::optional<db::record::user_data> rslt;
  with_wait_lock([this, &aid, &rslt] () {
      db::user_data_for_aid t(aid, rslt);
      pqxx_conn_->perform(t);
    });
  return rslt;
}
