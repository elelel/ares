#pragma once

namespace ares {
  namespace character {
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
    namespace zone_server {
      struct state;
      template <typename Packet>
      struct packet_handler;
    }
    namespace account_server {
      struct state;
      template <typename Packet>
      struct packet_handler;
    }
  }
}
