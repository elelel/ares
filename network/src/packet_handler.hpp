#pragma once

#include <ares/packets>

namespace ares {
  namespace network {
    template <typename Derived, typename Packet, typename Server, typename Session, typename SessionState>
    struct packet_handler {
      using handled_packet_type = Packet;
        
      packet_handler(Server& serv, Session& sess, SessionState& session_state, std::shared_ptr<Packet>);
      ~packet_handler();
      
      /*      size_t handle();
      void pop_packet();
      size_t packet_size() const; */
      
      std::shared_ptr<spdlog::logger> log() const;
      const elelel::network_buffer& send_buf() const;
    protected:
      std::shared_ptr<Packet> p_;

      Session& session_;
      Server& server_;
      SessionState& state_;
    };
    
  }
}
