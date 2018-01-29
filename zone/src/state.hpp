#pragma once

#include <rxcpp/rx.hpp>

#include "config.hpp"
#include "database/database.hpp"
#include "server.hpp"

#include "event/pc_move.hpp"

namespace ares {
  namespace zone {
    struct state {
      state();

      std::shared_ptr<asio::io_service> io_service() const;
      std::shared_ptr<spdlog::logger> log() const;
        
    private:
      std::shared_ptr<asio::io_service> io_service_;
      std::shared_ptr<spdlog::logger> log_;
        
    public:
      const config conf;
      database db;
      zone::server server;
        
      /*! Coordination to work on separate threads */
      decltype(rxcpp::observe_on_event_loop()) pool_thread = rxcpp::observe_on_event_loop();

      /*! Stream of PC move commands from client */
      rxcpp::rxsub::subject<event::pc_move_command> pc_move_commands;

      std::vector<std::string> map_names;
    };
  }
}
