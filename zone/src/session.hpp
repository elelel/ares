#pragma once

#include <array>
#include <chrono>
#include <memory>
#include <variant>

#include <spdlog/spdlog.h>

#include <ares/network>
#include <ares/packets>

#include "recv_handler.hpp"
#include "send_handler.hpp"

#include "mono/state.hpp"
#include "client/state.hpp"
#include "character_server/state.hpp"

namespace ares {
  namespace zone {
    struct state;
    
    struct session : ares::network::session<ares::zone::session>, std::enable_shared_from_this<ares::zone::session> {

      using state_variant = std::variant<mono::state, client::state, character_server::state>;

      struct inactivity_timer_handler : ares::network::handler::asio::base<inactivity_timer_handler, session> {
        using ares::network::handler::asio::base<inactivity_timer_handler, session>::base;
        void operator()(const std::error_code& ec);
      };

      friend struct recv_handler;
      friend struct send_handler;

      // Needed to allow changing of state type
      //friend struct mono::packet_handler<ares::packet::CH_ENTER>;
      //friend struct mono::packet_handler<ares::packet::ATHENA_ZH_LOGIN_REQ>;
      
      session(zone::state& zone_state, std::shared_ptr<asio::ip::tcp::socket> socket);

      void remove_from_server();
      /*! Defuses ASIO: terminates outstanding operations that depend on the lifetime of this object */
      void defuse_asio();

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
      bool is_character_server() const;
      character_server::state& as_character_server();
      bool is_client() const;
      client::state& as_client();

      recv_handler make_recv_handler();
      send_handler make_send_handler();

      /*! Standard packet obfuscation keys */
      uint32_t crypt_key[3]{0};

    private:
      state_variant session_state_;
    public:
      zone::state& server_state_;
    private:
      std::chrono::seconds inactivity_timeout_{120};
    };

    using session_ptr = std::shared_ptr<session>;
  }
}
