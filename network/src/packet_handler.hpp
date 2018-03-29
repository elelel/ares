#pragma once

#include <ares/packets>

namespace ares {
  namespace network {
    /*!
      Generic handler for specific packets, specialized later for Server/State
     */
    template <typename Derived, typename Packet, typename Server, typename Session, typename SessionState>
    struct packet_handler {
      using handled_packet_type = Packet;
        
      packet_handler(Server& serv, Session& sess, SessionState& session_state, std::shared_ptr<std::byte[]>);
      ~packet_handler();
      
      std::shared_ptr<spdlog::logger> log() const;
      const elelel::network_buffer& send_buf() const;
    protected:
      const Packet* p_;

      Session& session_;
      Server& server_;
      SessionState& state_;
      
      std::shared_ptr<std::byte[]> buf_;
    };
    
  }
}
