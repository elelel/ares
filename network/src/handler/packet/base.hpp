#pragma once

namespace ares {
  namespace network {
    namespace handler {
      namespace packet {
        template <typename Handler, typename Packet, typename Server, typename Session, typename State>
        struct base {
          base(Server& server, Session& session, State& state);
          ~base();

          void pop_packet();
          size_t packet_size() const;
          size_t handle();

        protected:
          Server& server_;
          Session& session_;
          State& state_;
          Packet* p_;
          bool need_pop_;
          
          std::shared_ptr<spdlog::logger>& log();
        };
      }
    }
  }
}
