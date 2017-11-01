#pragma once

namespace ares {
  namespace account {
    struct server;
    struct config;
    struct session;
    
    struct recv_handler;
    struct send_handler;
    
    namespace mono {
      struct state;
      template <typename Packet>
      struct packet_handler;
    }
    namespace client {
      struct state;
      template <typename Packet>
      struct packet_handler;
    }
    namespace char_server {
      struct state;
      template <typename Packet>
      struct packet_handler;
    }
  }
}
