#pragma once

#include <map>
#include <memory>
#include <set>
#include <thread>

#include <boost/asio.hpp>
#include <spdlog/spdlog.h>

#include "acceptor.hpp"

namespace ares {
  namespace network {
    template <typename Server>
    struct server {
      using type = server<Server>;
    
      using listen_type = boost::asio::ip::tcp::endpoint;
      using acceptor_type = acceptor<Server>;
    
      server(std::shared_ptr<spdlog::logger> log,
             std::shared_ptr<boost::asio::io_service> io_service,
             const size_t num_threads);

      // Interface with acceptor
      void create_session(std::shared_ptr<boost::asio::ip::tcp::socket> socket);

      std::mutex& mutex();
      std::shared_ptr<boost::asio::io_service> io_service() const;
      std::shared_ptr<spdlog::logger> log() const;
    protected:
      // Interface with Server
      void start(const listen_type& listen_info);
      void stop(const listen_type& listen_info);
      void stop();
    
      std::shared_ptr<spdlog::logger> log_;
      std::shared_ptr<boost::asio::io_service> io_service_;
      size_t num_threads_;
    private:
      std::mutex mutex_;

      std::unique_ptr<boost::asio::io_service::work> work_;
      std::unique_ptr<std::thread> main_thread_;
      std::vector<std::unique_ptr<std::thread>> thread_pool_; 

      std::set<std::shared_ptr<acceptor_type>> acceptors_;
    };
  }
} 
