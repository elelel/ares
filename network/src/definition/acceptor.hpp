#pragma once

#include "../acceptor.hpp"

template <typename Server>
inline ares::network::acceptor<Server>::acceptor(Server& server,
                                                     const asio::ip::tcp::endpoint& listen_ep) :
  server_(server),
  ep_(listen_ep),
  closed_(true) {
  try {
    using namespace asio;
    boost_acceptor_ = std::make_unique<ip::tcp::acceptor>(*server_.io_context(), ep_);
  } catch (std::system_error& e) {
    log()->error("Failed to create boost acceptor for address {} - error {}. {}", ep_.address().to_string(), e.code().value(), e.code().message());
    throw e;
  }
}

template <typename Server>
inline void ares::network::acceptor<Server>::start() {
  SPDLOG_TRACE(log(), "Starting TCP acceptor {0:#x}", (uintptr_t)this);
  try {
    if (!closed_ ) {
      try {
        boost_acceptor_->close();
      } catch (...) {
        log()->error("Failed to close already open boost acceptor");
      }
    }
    boost_acceptor_->listen();
    closed_ = false;
  } catch (std::system_error& e) {
    log()->error("Failed to listen on boost acceptor - error {}, {}", e.code().value(), e.code().message());
  }
  init();
}

template <typename Server>
inline void ares::network::acceptor<Server>::close() {
  closed_ = true;
}

template <typename Server>
inline bool ares::network::acceptor<Server>::closed() const {
  return closed_;
}

template <typename Server>
inline void ares::network::acceptor<Server>::init() {
  using namespace asio;
  auto socket = std::make_shared<ip::tcp::socket>(ip::tcp::socket(*server_.io_context()));
  try {
    boost_acceptor_->async_accept(*socket, [this, socket] (const std::error_code& ec) {
        if (ec.value() == 0) {
          SPDLOG_TRACE(log(), "Acceptor received request");
          std::lock_guard<std::mutex> lock(server_.mutex());
          static_cast<Server*>(&server_)->create_session(socket);
        } else {
          log()->error("Acceptor on_accept failed {}, {}", ec.value(), ec.message());
          delete this;
          return;
        }
        if (!closed_) {
          init();
        } else {
          log()->info("TCP acceptor {0:#x} closed", (uintptr_t)this);
          boost_acceptor_->close();
          delete this;
          return;
        }
      });
  } catch (std::error_code& ec) {
    log()->error("async_accept failed on boost acceptor {}, {}", ec.value(), ec.message());
    delete this;
    return;
  }
}

template <typename Server>
inline auto ares::network::acceptor<Server>::log() const -> std::shared_ptr<spdlog::logger> {
  return server_.log();
}
