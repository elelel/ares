#pragma once

#include <ares/database>

#include "../config.hpp"
#include "structs.hpp"

namespace ares {
  namespace account {
    struct database : ares::database<database> {
      database(std::shared_ptr<spdlog::logger> log,
               const config::postgres_config& conf);

      /*! Returns true if the account with the specified login exists
        \param login user's login to check
      */
      bool user_exists(const std::string& login);
      /*! Create user in account server
        \param login account login
        \param password account password
      */
      void create_user(const std::string& login, const std::string& password);
      /*! Returns true if the password is correct for a login
        \param login account login
        \param password password to check
      */ 
      bool password_matches(const std::string& login, const std::string& password);
      /*! Load user data from account database server by login
        \param login account login to load data for
      */
      std::optional<db::record::user_data> user_data_for_login(const std::string& login);
      /*! Load user data for account database server by account id
        \param aid account id to load data for
      */
      std::optional<db::record::user_data> user_data_for_aid(const uint32_t aid);
    };
  }
}
