#pragma once

#include "../server.hpp"

template <typename Derived, typename Session>
inline ares::network::server<Derived, Session>::server(std::shared_ptr<spdlog::logger> log,
                                                       std::shared_ptr<asio::io_context> io_context,
                                                       const size_t num_threads) :
  io_context_(io_context),
  log_(log),
  num_threads_(num_threads) {
  }

template <typename Derived, typename Session>
inline void ares::network::server<Derived, Session>::start(const asio::ip::tcp::endpoint& ep) {
  SPDLOG_TRACE(log(), "ares::network::server::start acquiring server lock");
  std::unique_lock<std::mutex> lock(mutex_);
  SPDLOG_TRACE(log(), "ares::network::server::start adding new acceptor");
  for (auto it = acceptors_.begin(); it != acceptors_.end();) {
    if (*it == nullptr) {
      it = acceptors_.erase(it);
    } else {
      ++it;
    }
  }
  auto a = std::make_shared<acceptor>(static_cast<Derived&>(*this), ep);
  acceptors_.insert(a);
  lock.unlock();
  a->start();
}

template <typename Derived, typename Session>
inline void ares::network::server<Derived, Session>::run() {
  auto work = asio::make_work_guard(io_context_);
  for (size_t i = 0; i < num_threads_; ++i) {
    auto th = std::unique_ptr<std::thread>(new std::thread([this] () {
          static_cast<Derived*>(this)->init_thread_local();
          io_context_->run();
          SPDLOG_TRACE(this->log(), "Asio worker thread terminating");
        }));
    thread_pool_.push_back(std::move(th));
  }
  // TODO: convert this to something normal like CV wait
  while (true) {
    std::this_thread::sleep_for(std::chrono::seconds{60});
  }
}

template <typename Derived, typename Session>
inline void ares::network::server<Derived, Session>::remove_session(std::shared_ptr<Session> s) {
  SPDLOG_TRACE(log(), "Removing session {}, ref count {}", (void*)s.get(), s.use_count());
  s->server_.remove(s);
}

template <typename Derived, typename Session>
inline void ares::network::server<Derived, Session>::stop(const asio::ip::tcp::endpoint& listen_ep) {
  auto found = acceptors_.find(listen_ep);
  if (found != acceptors_.end) {
    found->second.stop();
    acceptors_.erase(found);
  }
}

template <typename Derived, typename Session>
inline void ares::network::server<Derived, Session>::stop() {
  for (auto& p : acceptors_) {
    p.second.stop();
  }
  acceptors_.clear();
}

template <typename Derived, typename Session>
inline auto ares::network::server<Derived, Session>::log() const -> std::shared_ptr<spdlog::logger> {
  return log_;
}

template <typename Derived, typename Session>
inline auto ares::network::server<Derived, Session>::io_context() const -> std::shared_ptr<asio::io_context> {
  //  SPDLOG_TRACE(log(), "ares::network::server::io_context()");
  return io_context_;
}

template <typename Derived, typename Session>
inline std::mutex& ares::network::server<Derived, Session>::mutex() {
  return mutex_;
}
