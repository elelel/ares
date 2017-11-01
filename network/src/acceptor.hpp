#pragma once

#include <atomic>
#include <memory>

#include <boost/asio.hpp>
#include <spdlog/spdlog.h>

namespace ares {
  namespace network {
    template <typename Server>
    struct acceptor {
      acceptor(std::shared_ptr<spdlog::logger> log,
               Server& server,
               boost::asio::io_service& io_service,
               const boost::asio::ip::tcp::endpoint& listen_ep);

      void start();
      void close();
      bool closed() const;
      void init();             
    private:
      std::shared_ptr<spdlog::logger> log_;
      Server& server_;
      boost::asio::io_service& io_service_;
      boost::asio::ip::tcp::endpoint ep_;
    
      std::atomic<bool> closed_;
      std::unique_ptr<boost::asio::ip::tcp::acceptor> boost_acceptor_;
    };
  }
}
