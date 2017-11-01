#pragma once

#include <array>
#include <chrono>
#include <memory>

#include <spdlog/spdlog.h>

#include <ares/network>
#include <ares/packets>

#include "predeclare.hpp"

#include "state.hpp"
#include "recv_handler.hpp"
#include "send_handler.hpp"

namespace ares {
  namespace zone {
    struct session : ares::network::session<session>, std::enable_shared_from_this<session> {


      struct inactivity_timer_handler : ares::network::handler::asio::base<inactivity_timer_handler, session> {
        using ares::network::handler::asio::base<inactivity_timer_handler, session>::base;
        void operator()(const boost::system::error_code& ec);
      };

      friend struct recv_handler;
      friend struct send_handler;

      // Needed to allow changing of state type
      friend struct mono::packet_handler<ares::packet::CH_ENTER>;
      friend struct mono::packet_handler<ares::packet::ATHENA_ZH_LOGIN_REQ>;
      
      session(server& server, std::shared_ptr<boost::asio::ip::tcp::socket> socket);

      void remove_from_server();
      void on_disconnect();
      
      void on_open();
      void before_close();
      void on_connection_reset();
      void on_eof();
      void on_socket_error();
      void on_operation_aborted();

      void on_inactivity_timer();
      void reset_inactivity_timer();

      state_variant_type& state_variant();

      bool is_character_server() const;
      character_server::state& as_character_server();
      bool is_client() const;
      client::state& as_client();

      recv_handler make_recv_handler();
      send_handler make_send_handler();

      server& server_;
    private:
      state_variant_type state_;

      std::chrono::seconds inactivity_timeout_{120};
    };

    using session_ptr = std::shared_ptr<session>;
  }
}
