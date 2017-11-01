#pragma once

#include <ares/network>

#include "predeclare.hpp"

namespace ares {
  namespace zone {
    struct send_handler : ares::network::handler::asio::send<send_handler, session> {
      using ares::network::handler::asio::send<send_handler, session>::send;
      
      void terminate_session();
      void on_operation_aborted();
      void on_connection_reset();
      void on_eof();
      void on_socket_error();
    };
  }
}

