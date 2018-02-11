#pragma once

#include <array>
#include <chrono>
#include <memory>
#include <variant>

#include <ares/network>
// #include <ares/packets>

#include "mono/state.hpp"
#include "client/state.hpp"
#include "zone_server/state.hpp"
#include "account_server/state.hpp"

namespace ares {
  namespace character {
    struct server;
    
    struct session : ares::network::session<session, server>, std::enable_shared_from_this<session> {
      using state_variant = std::variant<mono::state, client::state, zone_server::state, account_server::state>;
      
      friend struct mono::packet_handler<packet::current<packet::CH_ENTER>>;
      friend struct mono::packet_handler<packet::current<packet::ATHENA_ZH_LOGIN_REQ>>;
      friend struct account_server::packet_handler<packet::current<packet::ATHENA_AH_AID_AUTH_RESULT>>;
      
      session(character::server& serv,
              const std::optional<asio::ip::tcp::endpoint> connect_ep,
              std::shared_ptr<asio::ip::tcp::socket> socket,
              const std::chrono::seconds idle_timer_timeout);

      state_variant& variant();

      bool is_mono() const;
      mono::state& as_mono();
      bool is_account_server() const;
      account_server::state& as_account_server();
      bool is_zone_server() const;
      zone_server::state& as_zone_server();
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
      state_variant session_state_;
    };

    using session_ptr = std::shared_ptr<session>;
  }
}
