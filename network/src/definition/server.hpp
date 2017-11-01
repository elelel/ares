#pragma once

#include "../server.hpp"

template <typename Server>
inline ares::network::server<Server>::server(std::shared_ptr<spdlog::logger> log,
                             std::shared_ptr<boost::asio::io_service> io_service,
                             const size_t num_threads) :
  log_(log),
  io_service_(io_service),
  num_threads_(num_threads),
  work_(new boost::asio::io_service::work(*io_service_)) {
  for (size_t i = 0; i < num_threads_; ++i) {
    auto th = std::unique_ptr<std::thread>(new std::thread([this] () {io_service_->run(); }));
    thread_pool_.push_back(std::move(th));
  }
}

template <typename Server>
inline void ares::network::server<Server>::start(const listen_type& ep) {
  SPDLOG_TRACE(log_, "ares::network::server::start acquiring server lock");
  
  std::unique_lock lock(mutex_);
  SPDLOG_TRACE(log_, "ares::network::server::start server lock acquired");
  for (auto it = acceptors_.begin(); it != acceptors_.end();) {
    const auto& r = *it;
    if (r == nullptr) {
      it = acceptors_.erase(it);
    } else {
      ++it;
    }
  }
  auto a = std::make_shared<acceptor_type>(log_, static_cast<Server&>(*this), *io_service_, ep);
  acceptors_.insert(a);
  lock.unlock();
  a->start();
}

template <typename Server>
inline void ares::network::server<Server>::stop(const listen_type& listen_ep) {
  auto found = acceptors_.find(listen_ep);
  if (found != acceptors_.end) {
    found->second.stop();
    acceptors_.erase(found);
  }
}

template <typename Server>
inline void ares::network::server<Server>::stop() {
  for (auto& p : acceptors_) {
    p.second.stop();
  }
  acceptors_.clear();
}

template <typename Server>
inline void ares::network::server<Server>::create_session(std::shared_ptr<boost::asio::ip::tcp::socket> socket) {
  SPDLOG_TRACE(log, "ares::network::server::create_session acquiring server lock");
  std::lock_guard lock(mutex_);
  SPDLOG_TRACE(log, "ares::network::server::create_session server lock acquired");
  static_cast<Server*>(this)->create_session(socket);
}

template <typename Server>
inline std::mutex& ares::network::server<Server>::mutex() {
  return mutex_;
}

template <typename Server>
inline std::shared_ptr<boost::asio::io_service> ares::network::server<Server>::io_service() const {
  return io_service_;
}

template <typename Server>
inline std::shared_ptr<spdlog::logger> ares::network::server<Server>::log() const {
  return log_;
}
