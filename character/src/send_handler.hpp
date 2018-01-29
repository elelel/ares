#pragma once

#include <ares/network>

namespace ares {
  namespace character {
    struct session;

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

