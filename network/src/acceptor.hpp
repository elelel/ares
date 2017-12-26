#pragma once

/*! \file acceptor.hpp
  \brief tcp acceptor abstraction over ASIO acceptor
*/

#include <atomic>
#include <memory>

#include <boost/asio.hpp>
#include <spdlog/spdlog.h>

namespace ares {
  namespace network {
    /*! ASIO acceptor abstraction for Ares servers
      \tparam Server concrete server type
    */
    template <typename Server>
    struct acceptor {
      /*! Construct an acceptor for the server.
        \param log pointer to the logger
        \param server reference to the server
        \param io_service reference to ASIO service
        \param listen_ep ASIO endpoint which describes what TCP address and port to listen on
       */
      acceptor(std::shared_ptr<spdlog::logger> log,
               Server& server,
               boost::asio::io_service& io_service,
               const boost::asio::ip::tcp::endpoint& listen_ep);

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
