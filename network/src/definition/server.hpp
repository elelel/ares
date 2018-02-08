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
  std::unique_lock lock(mutex_);
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
          io_context_->run();
          SPDLOG_TRACE(this->log(), "Asio worker thread terminating");
        }));
    thread_pool_.push_back(std::move(th));
  }

  {
    using namespace rxcpp;
    using namespace rxcpp::rxo;
    auto close_gracefuly = close_gracefuly_stream.get_observable().observe_on(server_rxthreads)
      | tap([this] (auto&) {
          SPDLOG_TRACE(log(), "RX close_gracefuly");
        })
      | delay(std::chrono::milliseconds(100))
      | tap([this] (session_ptr s) {
          s->socket_->cancel();
          s->idle_timer_->cancel();
          s->connected_ = false;
          s->socket_->close();
        })
      | delay(std::chrono::seconds{2})
      | publish()
      | ref_count();

    auto close_abruptly = close_gracefuly 
      | tap([this] (session_ptr s) {
          SPDLOG_TRACE(log(), "RX close_abruptly");
          s->socket_->close();
          if (s->reconnect_timer_)
            s->set_reconnect_timer(s->reconnect_timer_timeout_, s->reconnect_timer_timeout_);
          s->server_.remove(s);
        })
      | publish()
      | connect_forever();

    while (true) {
      std::this_thread::sleep_for(std::chrono::seconds{1});
    }

  }

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
inline void ares::network::server<Derived, Session>::create_session(std::shared_ptr<asio::ip::tcp::socket> socket) {
  SPDLOG_TRACE(log(), "ares::network::server::create_session calling specialized create_session");
  static_cast<Derived*>(this)->create_session(socket);
}

template <typename Derived, typename Session>
inline void ares::network::server<Derived, Session>::close_gracefuly(session_ptr s) {
  SPDLOG_TRACE(log(), "ares::network::server::close_gracefuly");
  rxcpp::observable<>::create<session_ptr>([s] (rxcpp::subscriber<session_ptr> sub) {
      sub.on_next(s);
    }).subscribe_on(server_rxthreads).subscribe(close_gracefuly_stream.get_subscriber());
}

template <typename Derived, typename Session>
inline void ares::network::server<Derived, Session>::close_abruptly(session_ptr s) {
  SPDLOG_TRACE(log(), "ares::network::server::close_abruptly");
  rxcpp::observable<>::create<session_ptr>([s] (rxcpp::subscriber<session_ptr> sub) {
      sub.on_next(s);
    }).subscribe_on(server_rxthreads).subscribe(close_abruptly_stream.get_subscriber());
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

template <typename Derived, typename Session>
template <typename F>
inline void ares::network::server<Derived, Session>::on_rxthreads(F f) {
  using namespace rxcpp;
  observable<>::empty<int>(server_rxthreads).subscribe(make_subscriber<int>([] (int) {}, f));
}
                         
