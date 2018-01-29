#pragma once

/*! \file server.hpp
  \brief tcp server abstraction for Ares servers
*/

#include <map>
#include <memory>
#include <set>
#include <thread>

#include <asio.hpp>
#include <spdlog/spdlog.h>

#include "acceptor.hpp"

namespace ares {
  namespace network {
    /*! tcp server abstraction for Ares servers
      \tparam Server concrete server type
    */
    template <typename Server>
    struct server {
      using type = server<Server>;
    
      using listen_type = asio::ip::tcp::endpoint;
      using acceptor_type = acceptor<Server>;

      /*! Constructs the server
        \param log pointer to the logger
        \param io_service pointer to ASIO io_service
        \param num_threads number of threads dedicated to TCP listening
      */
      server(std::shared_ptr<spdlog::logger> log,
             std::shared_ptr<asio::io_service> io_service,
             const size_t num_threads);

      /*! Creates a network session within the server, called from acceptor
        \param socket TCP socket of the new network session
      */
      void create_session(std::shared_ptr<asio::ip::tcp::socket> socket);

      /*! Returns reference for server mutex which is used for server-wide synchronization
       */
      std::mutex& mutex();
      /*! Returns shared pointer to ASIO io_service
       */
      std::shared_ptr<asio::io_service> io_service() const;
      /*! Returns shared pointer to log service
       */
      std::shared_ptr<spdlog::logger> log() const;
    protected:
      /*! Start the generic server, called from the concrete Server descendent
        \param listen_info network endpoint to listen on
       */
      void start(const listen_type& listen_info);
      /*! Stops the generic server on an endpoint, called from the concrete Server descendent
        \param listen_info network endpoint to stop listening on
       */
      void stop(const listen_type& listen_info);
      /*! Stops the generic server, called from the concrete Server descendent
       */
      void stop();
    
      std::shared_ptr<spdlog::logger> log_;
      std::shared_ptr<asio::io_service> io_service_;
      size_t num_threads_;
    private:
      std::mutex mutex_;

      std::unique_ptr<asio::io_service::work> work_;
      std::unique_ptr<std::thread> main_thread_;
      std::vector<std::unique_ptr<std::thread>> thread_pool_; 

      std::set<std::shared_ptr<acceptor_type>> acceptors_;
    };
  }
} 
