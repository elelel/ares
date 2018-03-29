#pragma once

#include <memory>
#include <mutex>

#include <pqxx/pqxx>
#include <spdlog/spdlog.h>

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


      template <typename Transactor, typename... Args>
      inline auto query(Args&&... args) -> typename Transactor::result_type {
        typename Transactor::result_type rslt;
        try {
          Transactor t(rslt, std::forward<Args>(args)...);
          pqxx_conn_->perform(t);
        } catch (pqxx::failure& e) {
          log_->error(e.what());
          throw;
        } catch (...) {
          log_->error("Unknown exception while accessing database");
          throw;
        }
        return rslt;
      }

      template <typename Transactor, typename... Args>
      inline void exec(Args&&... args) {
        try {
          Transactor t(std::forward<Args>(args)...);
          pqxx_conn_->perform(t);
        } catch (pqxx::failure& e) {
          log_->error(e.what());
          throw;
        } catch (...) {
          log_->error("Unknown exception while accessing database");
          throw;
        }
      }

    private:
      std::shared_ptr<spdlog::logger> log_;
      std::unique_ptr<pqxx::connection> pqxx_conn_;
      size_t max_retry_{10};
    };

  }
}
