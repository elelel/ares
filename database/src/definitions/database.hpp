#include "../database.hpp"

template <typename Database>
inline ares::database<Database>::database(std::shared_ptr<spdlog::logger> log,
                                   const std::string& dbname,
                                   const std::string& host,
                                   const uint16_t port,
                                   const std::string& user,
                                   const std::string& password) :
  log_(log) {
  SPDLOG_TRACE(log, "ares::database::database");
  std::string conn_str;
  if (host != "") conn_str += " host=" + host;
  if (port != 0) conn_str += " port=" + std::to_string(port);
  if (dbname != "") conn_str += " dbname=" + dbname;
  if (user != "") conn_str += " user=" + user;
  if (password != "") conn_str += " password=" + password;

  try {
    pqxx_conn_ = std::unique_ptr<pqxx::connection>(new pqxx::connection(conn_str));
  } catch (std::exception& e) {
    log_->error("Could not open PostgreSQL database, " + std::string(e.what()));
    throw e;
  }
  SPDLOG_TRACE(log, "ares::database::database end");
}

template <typename Database>
inline void ares::database<Database>::with_wait_lock(std::function<void()> f) {
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
