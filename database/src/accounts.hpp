#pragma once

#include <pqxx/pqxx>

#include <ares/model>

#include "pqxx_traits.hpp"

namespace ares {
  namespace database {
    namespace accounts {
      namespace record {
        /*! User data record in account server's database
         */
        struct user_data {
          /*! Account id */
          model::account_id account_id = model::account_id::from_uint32(0);
          /*! Account login */
          std::string login;
          /*! Account email */
          std::string email;
          /*! Account level */
          uint32_t level;
          /*! Account sex */
          uint8_t sex;
          /*! Account expiration date/time */
          std::optional<std::chrono::system_clock::time_point> expiration_time;
          /*! Account birthdate */
          std::optional<std::chrono::system_clock::time_point> birthdate;
          /*! Account pin */
          std::optional<std::string> pin;
        };
      
        /*! Account slots info in character server's database
         */
        struct account_slots {
          model::account_id account_id = model::account_id::from_uint32(0);
          uint8_t normal_slots;
          uint8_t premium_slots;
          uint8_t billing_slots;
          uint8_t creatable_slots;
          uint8_t playable_slots;
        };

        /*! Account storage info in character server's database
         */
        struct account_storage {
          uint32_t bank_vault;
          uint32_t max_storage;
        };
      }

      /*! Returns true if the account with the specified login exists

      */
      struct exists : pqxx::transactor<> {
        using result_type = bool;
        result_type& rslt;

        /*! \param login user's login to check for existence
         */
        exists(result_type& rslt, const std::string& login);

        void operator()(argument_type& trans);
      private:
        const std::string& login_;
      };

      /*! Create user account
      */
      struct create : pqxx::transactor<> {
        /*!\param login account login
          \param password account password */
        create(const std::string& login, const std::string& password, const std::string& email, const uint8_t level);

        void operator()(argument_type& trans);
      private:
        const std::string& login_;
        const std::string& password_;
        const std::string& email_;
        const uint8_t level_;
      };

      struct create_data : pqxx::transactor<> {
        create_data(const model::account_id& account_id,
                    const uint8_t& normal_slots,
                    const uint8_t& premium_slots,
                    const uint8_t& billing_slots,
                    const uint8_t& creatable_slots,
                    const uint8_t& playable_slots,
                    const uint32_t& bank_vault,
                    const uint32_t& max_storage
                    );

        void operator()(argument_type& trans);
      private:
        const model::account_id& account_id_;
        const uint8_t& normal_slots_;
        const uint8_t& premium_slots_;
        const uint8_t& billing_slots_;
        const uint8_t& creatable_slots_;
        const uint8_t& playable_slots_;
        const uint32_t& bank_vault_;
        const uint32_t& max_storage_;        
      };

      /*! Returns true if the password is correct for a login
       */ 

      struct password_matches : pqxx::transactor<> {
        using result_type = bool;
        result_type& rslt;
        
        /*! 
          \param login account login
          \param password password to check
        */
        password_matches(result_type& rslt, const std::string& login, const std::string& password);
        
        void operator()(argument_type& trans);
        
      private:
        const std::string& login_;
        const std::string& password_;
      };

      struct num_chars : pqxx::transactor<> {
        using result_type = std::optional<size_t>;
        result_type& rslt;

        num_chars(result_type& rslt, const model::account_id& account_id, const size_t& max_chars);

        void operator()(argument_type& trans);
      private:
        const model::account_id& account_id_;
        const size_t& max_chars_;
      };

      /*! Load account slots record for a given account_id */
      struct slots : pqxx::transactor<> {
        using result_type = std::optional<record::account_slots>;
        result_type& rslt;

        slots(result_type& rslt, const model::account_id account_id);

        void operator()(argument_type& trans);
      private:
        model::account_id account_id_;
      };

      /*! Load account storage record for a given account_id */
      struct storage : pqxx::transactor<> {
        using result_type = std::optional<record::account_storage>;
        result_type& rslt;

        storage(result_type& rslt, const model::account_id account_id);

        void operator()(argument_type& trans);
      private:
        model::account_id account_id_;
      };

      /*! Load user data from account database server by login
       */
      struct user_data_for_login : pqxx::transactor<> {
        using result_type = std::optional<record::user_data>;
        result_type& rslt;

        /*! \param login account login to load data for */
        user_data_for_login(result_type& rslt, const std::string& login);

        void operator()(argument_type& trans);
      private:
        const std::string& login_;
      };

      /*! Load user data for account database server by account id
       */
      struct user_data_for_id : pqxx::transactor<> {
        using result_type = std::optional<record::user_data>;
        result_type& rslt;

        /*!
          \param id account id to load data for
        */
        user_data_for_id(result_type& rslt, const model::account_id& account_id);

        void operator()(argument_type& trans);
      private:
        const model::account_id& account_id_;
        
      };
    }
  }
}
