#pragma once

#include <chrono>
#include <variant>

#include <ares/network>

#include "mono/state.hpp"
#include "client/state.hpp"
#include "character_server/state.hpp"

namespace ares {
  namespace account {
    struct server;
    
    struct session : ares::network::session<session, server>, std::enable_shared_from_this<session> {

      using state_variant = std::variant<mono::state, client::state, character_server::state>;
      
      session(ares::account::server& serv,
              const std::optional<asio::ip::tcp::endpoint> connect_ep,
              std::shared_ptr<asio::ip::tcp::socket> socket,
              const std::chrono::seconds idle_timer_timeout);

      state_variant& variant();

      bool is_mono() const;
      mono::state& as_mono();
      bool is_char_server() const;
      character_server::state& as_char_server();
      bool is_client() const;
      client::state& as_client();

      // Session handlers
      void on_connect();
      void on_connection_reset();
      void on_operation_aborted();
      void on_eof();
      void on_socket_error();
      void on_packet_processed();
      void defuse_asio();
      
      packet::alloc_info allocate(const uint16_t packet_id);
      void dispatch_packet(const uint16_t packet_id,
                           void* buf,
                           std::function<void(void*)> deallocator);

    private:
      friend struct mono::packet_handler<packet::current<packet::ATHENA_HA_LOGIN_REQ>>;
      
      state_variant session_state_;
    };

    using session_ptr = std::shared_ptr<session>;

  }
}
