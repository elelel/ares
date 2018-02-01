#pragma once

#include <array>
#include <chrono>
#include <memory>
#include <variant>

#include <ares/network>
#include <ares/packets>

#include "recv_handler.hpp"
#include "send_handler.hpp"

#include "mono/state.hpp"
#include "client/state.hpp"
#include "zone_server/state.hpp"
#include "account_server/state.hpp"

namespace ares {
  namespace character {
    struct session : ares::network::session<session>, std::enable_shared_from_this<session> {
      using state_variant = std::variant<mono::state, client::state, zone_server::state, account_server::state>;
      
      struct inactivity_timer_handler : ares::network::handler::asio::base<inactivity_timer_handler, session> {
        using ares::network::handler::asio::base<inactivity_timer_handler, session>::base;
        void operator()(const std::error_code& ec);
      };

      friend struct recv_handler;
      friend struct send_handler;

      // Needed to allow changing of state type
      friend struct mono::packet_handler<packet<packets::CH_ENTER>>;
      friend struct mono::packet_handler<packet<packets::ATHENA_ZH_LOGIN_REQ>>;
      friend struct account_server::packet_handler<packet<packets::ATHENA_AH_AID_AUTH_RESULT>>;
      
      session(character::state& server_state, std::shared_ptr<asio::ip::tcp::socket> socket);

      void defuse_asio();
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

      state_variant& variant();

      bool is_mono() const;
      mono::state& as_mono();
      bool is_account_server() const;
      account_server::state& as_account_server();
      bool is_zone_server() const;
      zone_server::state& as_zone_server();
      bool is_client() const;
      client::state& as_client();

      recv_handler make_recv_handler();
      send_handler make_send_handler();

    private:
      state_variant session_state_;
    public:
      character::state& server_state_;
      std::chrono::seconds inactivity_timeout_{120};
    };

    using session_ptr = std::shared_ptr<session>;
  }
}
