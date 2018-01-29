#pragma once

#include "../acceptor.hpp"

template <typename Server>
inline ares::network::acceptor<Server>::acceptor(std::shared_ptr<spdlog::logger> log,
                         Server& server,
                         asio::io_service& io_service,
                         const asio::ip::tcp::endpoint& listen_ep) :
  log_(log),
  server_(server),
  io_service_(io_service),
  ep_(listen_ep),
  closed_(true) {
  try {
    using namespace asio;
    boost_acceptor_ = std::make_unique<ip::tcp::acceptor>(io_service_, ep_);
  } catch (std::system_error& e) {
    log_->error("Failed to create boost acceptor for address {} - error {}. {}", ep_.address().to_string(), e.code().value(), e.code().message());
    throw e;
  }
}

template <typename Server>
inline void ares::network::acceptor<Server>::start() {
  SPDLOG_TRACE(log_, "Starting TCP acceptor {0:#x}", (uintptr_t)this);
  try {
    if (!closed_ ) {
      try {
        boost_acceptor_->close();
      } catch (...) {
        log_->error("Failed to close already open boost acceptor");
      }
    }
    boost_acceptor_->listen();
    closed_ = false;
  } catch (std::system_error& e) {
    log_->error("Failed to listen on boost acceptor - error {}, {}", e.code().value(), e.code().message());
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
  auto socket = std::make_shared<ip::tcp::socket>(ip::tcp::socket(io_service_));
  try {
    boost_acceptor_->async_accept(*socket, [this, socket] (const std::error_code& ec) {
        if (ec.value() == 0) {
          SPDLOG_TRACE(log_, "Acceptor received request");
          static_cast<Server*>(&server_)->create_session(socket);
        } else {
          log_->error("Acceptor on_accept failed {}, {}", ec.value(), ec.message());
          delete this;
          return;
        }
        if (!closed_) {
          init();
        } else {
          log_->info("TCP acceptor {0:#x} closed", (uintptr_t)this);
          boost_acceptor_->close();
          delete this;
          return;
        }
      });
  } catch (std::error_code& ec) {
    log_->error("async_accept failed on boost acceptor {}, {}", ec.value(), ec.message());
    delete this;
    return;
  }
}
