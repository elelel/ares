#pragma once

#include <spdlog/spdlog.h>

#include <ares/network>

#include "../predeclare.hpp"

namespace ares {
  namespace character {
    namespace zone_server {
      struct state {
        state(std::shared_ptr<spdlog::logger> log, server& serv, session& sess);        
        state(const mono::state& mono_state);

        // Interface with character::session
        void on_open();
        void before_close();
        void on_connection_reset();
        void on_eof();
        void on_socket_error();
        void on_operation_aborted();
        size_t dispatch(const uint16_t PacketType);
        void defuse_asio();

        // Data
        std::string login;
        std::vector<uint32_t> map_ids;
        
        uint32_t ip;
        uint16_t port;

        std::map<uint32_t, uint32_t> online_aid_to_cid;
        std::map<uint32_t, uint32_t> online_cid_to_aid;
        uint32_t base_rate{0};
        uint32_t job_rate{0};
        uint32_t drop_rate{0};

      private:
        std::shared_ptr<spdlog::logger> log_;
        server& server_;
        session& session_;

      };
    }
  }
}
