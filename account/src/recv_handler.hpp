#pragma once

#include <ares/network>

namespace ares {
  namespace account {
    struct session;
    
    struct recv_handler : ares::network::handler::asio::receive<recv_handler, session> {
      using ares::network::handler::asio::receive<recv_handler, session>::receive;
      
      size_t dispatch(const uint16_t PacketType);

      void on_processed_packet();
      void terminate_session();
      void on_operation_aborted();
      void on_connection_reset();
      void on_eof();
      void on_socket_error();
      void on_inactivity_timeout();
    };
  }
}
