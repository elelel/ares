#pragma once

#include <spdlog/spdlog.h>

#include <ares/network>

#include "../predeclare.hpp"

namespace ares {
  namespace account {
    namespace char_server {
      /*! State for sessions that are character servers */
      struct state {
        state(std::shared_ptr<spdlog::logger> log, server& serv, session& sess);
        /*! Constructor to create character server state from monostate
          \param mono_state monostate to create character server state from */
        state(const mono::state& mono_state);
        
        // Interface with account::session
        void defuse_asio();
        void on_open();
        void before_close();
        void on_connection_reset();
        void on_eof();
        void on_socket_error();
        void on_operation_aborted();
        size_t dispatch(const uint16_t PacketType);

        // Data
        /*! Characer server's login */
        std::string login;
        /*! Character server's name */
        std::string name;
        /*! Character server's IPv4 address */
        boost::asio::ip::address_v4 ip_v4;
        /*! Character server's IPv4 port */
        uint16_t port;
        uint16_t state_num;
        uint16_t property;

        /*! Number of users online in this character server */
        uint32_t user_count{0};

      private:
        std::shared_ptr<spdlog::logger> log_;
        server& server_;
        session& session_;
      };
    }
  }
}
