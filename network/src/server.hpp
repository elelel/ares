#pragma once

#include <mutex>
#include <set>

#include <spdlog/spdlog.h>

#include <asio/ts/io_context.hpp>
#include <asio/ts/executor.hpp>
#include <asio/ts/socket.hpp>
#include <asio/ts/net.hpp>

#include "acceptor.hpp"

namespace ares {
  namespace network {
    /*!
      Generic Ares server
    */
    template <typename Derived, typename Session>
    struct server {
      using acceptor = ares::network::acceptor<Derived>;

      /*! Constructs server
        \param io_context pointer to ASIO io_context
        \param num_threads number of threads dedicated to TCP listening
      */
      server(std::shared_ptr<spdlog::logger> log,
             std::shared_ptr<asio::io_context> io_context,
             const size_t num_threads);

      /*! Returns shared pointer to log service
       */
      std::shared_ptr<spdlog::logger> log() const;
      /*! Returns shared pointer to server io_context
       */
      std::shared_ptr<asio::io_context> io_context() const;
      /*! Returns reference for server mutex which is used for server-wide synchronization
       */
      std::mutex& mutex();
      /*! Creates a network session within the server, called from acceptor
        \param socket TCP socket of the new network session
      */
      //      void create_session(std::shared_ptr<asio::ip::tcp::socket> socket);
      
      void remove_session(std::shared_ptr<Session> s);

      void run();

    protected:

      /*! Start the generic server, called from the concrete Derived descendant
        \param listen_info network endpoint to listen on
      */
      void start(const asio::ip::tcp::endpoint& ep);
      /*! Stops the generic server on an endpoint, called from the concrete Derived descendant
        \param listen_info network endpoint to stop listening on
      */
      void stop(const asio::ip::tcp::endpoint& listen_info);
      /*! Stops the generic server, called from the concrete Derived descendant
       */
      void stop();
      
    protected:
      std::shared_ptr<asio::io_context> io_context_;
      std::shared_ptr<spdlog::logger> log_;
      std::mutex mutex_;
      size_t num_threads_;

    private:
      friend Derived;
      
      std::unique_ptr<std::thread> main_thread_;
      std::vector<std::unique_ptr<std::thread>> thread_pool_;
      std::set<std::shared_ptr<acceptor>> acceptors_;

    };
  }
}
