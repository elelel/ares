#pragma once

#include <ares/packets>

namespace ares {
  namespace network {
    namespace handler {
      namespace packet {
        template <typename Handler,
                  typename PacketSet,
                  typename PacketName,
                  typename ServerState,
                  typename Session,
                  typename SessionState>
        struct base {
          template <typename PacketName_ = PacketName>
          using packet_type = ares::packet::type<PacketSet, PacketName_>;
          using handled_packet_type = ares::packet::type<PacketSet, PacketName>;
          
          base(ServerState& server, Session& session, SessionState& session_state);
          ~base();

          void pop_packet();
          size_t packet_size() const;
          size_t handle();

        protected:
          ServerState& server_state_;
          Session& session_;
          SessionState& session_state_;
          packet_type<>* p_;
          bool need_pop_;
          
          std::shared_ptr<spdlog::logger>& log();
        };
      }
    }
  }
}
