#pragma once

#include <condition_variable>
#include <memory>
#include <mutex>

#include <pqxx/pqxx>
#include <spdlog/spdlog.h>

#include "pqxx_traits.hpp"
#include "account/records.hpp"
#include "character/records.hpp"
#include "zone/records.hpp"

namespace ares {
  namespace database {
    struct db {
      /* Constructor that connects to Postgresql database
         \param log pointer to logger
         \param dbname string containing database name
         \param host string containing database server host
         \param port TCP port database server is listening on
         \param username database username
         \param password database password
      */
      db(std::shared_ptr<spdlog::logger> log,
         const std::string& dbname,
         const std::string& host,
         const uint16_t port,
         const std::string& username,
         const std::string& password);

      /*! Returns true if the account with the specified login exists
        \param login user's login to check
      */
      bool user_exists(const std::string& login);
      /*! Create user in account server
        \param login account login
        \param password account password
      */
      void create_user(const std::string& login, const std::string& password, const std::string& email, const uint8_t level);
      /*! Returns true if the password is correct for a login
        \param login account login
        \param password password to check
      */ 
      bool password_matches(const std::string& login, const std::string& password);
      /*! Load user data from account database server by login
        \param login account login to load data for
      */
      std::optional<record::user_data> user_data_for_login(const std::string& login);
      /*! Load user data for account database server by account id
        \param aid account id to load data for
      */
      std::optional<record::user_data> user_data_for_aid(const uint32_t aid);


      /*! Create account data record for an aid */
      void account_create(const uint32_t aid,
                          const uint8_t normal_slots,
                          const uint8_t premium_slots,
                          const uint8_t billing_slots,
                          const uint8_t creatable_slots,
                          const uint8_t playable_slots,
                          const uint32_t bank_vault,
                          const uint32_t max_storage);

      /*! Load account slots record for a given aid */
      std::optional<record::account_slots> account_slots_for_aid(const uint32_t aid);

      /*! Load account slots record for a given aid */
      std::optional<record::account_storage> account_storage_for_aid(const uint32_t aid);

      /*! Create a new character */
      std::optional<uint32_t> char_create(const uint32_t aid,
                                          const uint8_t slot,
                                          const std::string& name,
                                          const uint8_t sex);

      std::optional<size_t> num_chars_for_aid(const uint32_t aid, const size_t max_chars);
      
      std::vector<model::pc_info> character_info_for_aid(const uint32_t aid, const size_t max_chars);

      std::optional<model::pc_info> character_info(const uint32_t cid);

      std::optional<model::pc_info> character_info_for_slot(const uint32_t aid, const uint16_t slot);

      std::optional<std::chrono::system_clock::time_point> char_delete_date(const uint32_t cid);
      
      std::optional<uint32_t> make_char(const uint32_t aid,
                                        const std::string& name,
                                        const uint8_t slot,
                                        const uint16_t head_palette,
                                        const uint16_t head,
                                        const model::pc_job job,
                                        const uint8_t sex,
                                        const uint64_t zeny,
                                        const uint32_t map_id,
                                        const uint16_t map_x,
                                        const uint16_t map_y);

      std::vector<record::map_index> map_name_index();
      std::optional<uint32_t> map_id_by_name(const std::string& name);

      std::optional<model::map_info> map_info(const size_t map_id);
      
      std::optional<uint32_t> recreate_map(const std::string& map_name, const model::map_info& map_info);
      
    private:
      /*! Execute function under database mutex lock, that will ensure sequencing and thread safety
        \param f function to execute under the database mutex lock
      */
      inline void with_wait_lock(std::function<void()> f) {
        std::unique_lock<std::mutex> lock(mutex_);
        cv_.wait(lock, [this] { return ready_; });
        try {
          ready_ = false;
          f();
          ready_ = true;
          lock.unlock();
          cv_.notify_one();
        } catch (pqxx::failure& e) {
          log_->error(e.what());
          ready_ = true;
          lock.unlock();
          cv_.notify_one();
          throw(e);
        } catch (...) {
          log_->error("Unknown exception while accessing database");
          ready_ = true;
          lock.unlock();
          cv_.notify_one();
          throw;
        }
      }

      std::shared_ptr<spdlog::logger> log_;
      std::condition_variable cv_;
      bool ready_{true};
      std::mutex mutex_;
      std::unique_ptr<pqxx::connection> pqxx_conn_;
      size_t max_retry_{10};
    };      
  }
}
