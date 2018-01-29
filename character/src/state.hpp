#pragma once

#include "config.hpp"
#include "database/database.hpp"
#include "server.hpp"

namespace ares {
  namespace character {
    struct state {
      state();
      std::shared_ptr<boost::asio::io_service> io_service() const;
      std::shared_ptr<spdlog::logger> log() const;

      // Data
      uint16_t state_num{0};
      uint16_t property{0};
    private:
      std::shared_ptr<boost::asio::io_service> io_service_;
      std::shared_ptr<spdlog::logger> log_;

    public:
      /*! account server config */
      const config conf;
      /*! account server database */
      database db;
      character::server server;
    };

  }
}
