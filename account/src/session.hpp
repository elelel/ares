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
      void defuse_asio();
      
      std::tuple<size_t, size_t, size_t> packet_sizes(uint16_t& packet_id);
      void dispatch_packet(std::shared_ptr<std::byte[]> buf);

    private:
      friend struct mono::packet_handler<packet::current<packet::ARES_HA_LOGIN_REQ>>;
      
      state_variant session_state_;
    };

    inline bool operator<(const std::weak_ptr<session>& lhs,
                          const std::weak_ptr<session>& rhs) {
      auto l = lhs.lock();
      auto r = rhs.lock();
      if (l && r) return l->id() < r->id();
      if (l && !r) return false;
      if (!l && r) return true;
      return false;
    }

  }
}
