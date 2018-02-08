#pragma once

#include <ares/packets>

namespace ares {
  namespace network {
    template <typename Derived, typename PacketSet, typename PacketName, typename Server, typename Session, typename SessionState>
    struct packet_handler {
      using handled_packet_type = packet::type<PacketSet, PacketName>;
      template <typename PacketName_>
      using packet_type = packet::type<PacketSet, PacketName>;
        
      packet_handler(Server& serv, Session& sess, SessionState& session_state);
      ~packet_handler();
      
      size_t handle();
      void pop_packet();
      size_t packet_size() const;
      
      std::shared_ptr<spdlog::logger> log() const;
      const elelel::network_buffer& recv_buf() const;
      const elelel::network_buffer& send_buf() const;
    protected:
      handled_packet_type* p_;

      Session& session_;
      Server& server_;
      SessionState& state_;
      
    private:
      bool need_pop_;
    };
    
  }
}
