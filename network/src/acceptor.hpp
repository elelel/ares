#pragma once

/*! \file acceptor.hpp
  \brief tcp acceptor abstraction over ASIO acceptor
*/

#include <atomic>
#include <memory>

#include <spdlog/spdlog.h>

#include <asio/ts/io_context.hpp>
#include <asio/ts/executor.hpp>
#include <asio/ts/socket.hpp>
#include <asio/ts/net.hpp>

namespace ares {
  namespace network {
    /*! ASIO acceptor abstraction for Ares servers
      \tparam Server concrete server type
    */
    template <typename Server>
    struct acceptor {
      /*! Construct an acceptor for the server.
        \param server reference to the server
        \param listen_ep ASIO endpoint which describes what TCP address and port to listen on
       */
      acceptor(Server& server,
               const asio::ip::tcp::endpoint& listen_ep);

      /*! Starts the acceptor, if the acceptor has already started, closes previous ASIO acceptor
       */
      void start();
      /*! Puts the acceptor into closed state
       */
      void close();
      /*! Checks if acceptor is closed
       */ 
      bool closed() const;
      /*! Starts accepting new connections with ASIO's async_accept
       */
      void init();

      /*! Returns shared pointer to log service
       */
      std::shared_ptr<spdlog::logger> log() const;
      
    private:
      Server& server_;
      asio::ip::tcp::endpoint ep_;
    
      std::atomic<bool> closed_;
      std::unique_ptr<asio::ip::tcp::acceptor> boost_acceptor_;
    };
  }
}
