#pragma once

#include "config.hpp"
#include "database/database.hpp"
#include "server.hpp"

namespace ares {
  namespace account {
    struct state {
      state();
      
      std::shared_ptr<boost::asio::io_service> io_service() const;
      std::shared_ptr<spdlog::logger> log() const;
        
    private:
      std::shared_ptr<boost::asio::io_service> io_service_;
      std::shared_ptr<spdlog::logger> log_;

    public:
      /*! account server config */
      const config conf;
      /*! account server database */
      database db;
      account::server server;
      
    };
  }
}
