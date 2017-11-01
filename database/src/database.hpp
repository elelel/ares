#pragma once

#include <condition_variable>
#include <memory>
#include <mutex>
#include <optional>

#include <pqxx/pqxx>
#include <spdlog/spdlog.h>

#include "pqxx_traits.hpp"

namespace ares {
  template <typename Database>
  struct database {
  protected:
    database(std::shared_ptr<spdlog::logger> log,
             const std::string& dbname,
             const std::string& host,
             const uint16_t port,
             const std::string& username,
             const std::string& password);

    void with_wait_lock(std::function<void()> f);

    std::shared_ptr<spdlog::logger> log_;
    std::condition_variable cv_;
    bool ready_{true};
    std::mutex mutex_;
    std::unique_ptr<pqxx::connection> pqxx_conn_;
    size_t max_retry_{10};
  };
}
