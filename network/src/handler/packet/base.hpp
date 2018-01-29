#pragma once

namespace ares {
  namespace network {
    namespace handler {
      namespace packet {
        template <typename Handler, typename Packet, typename ServerState, typename Session, typename SessionState>
        struct base {
          base(ServerState& server, Session& session, SessionState& session_state);
          ~base();

          void pop_packet();
          size_t packet_size() const;
          size_t handle();

        protected:
          ServerState& server_state_;
          Session& session_;
          SessionState& session_state_;
          Packet* p_;
          bool need_pop_;
          
          std::shared_ptr<spdlog::logger>& log();
        };
      }
    }
  }
}
