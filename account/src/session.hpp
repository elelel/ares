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
#include "char_server/state.hpp"

namespace ares {
  namespace account {
    struct state;

    /*! Session descriptor for account server sessions */
    struct session : ares::network::session<session>, std::enable_shared_from_this<session> {
      using state_variant = std::variant<mono::state, client::state, char_server::state>;
      
      struct inactivity_timer_handler : ares::network::handler::asio::base<inactivity_timer_handler, session> {
        using ares::network::handler::asio::base<inactivity_timer_handler, session>::base;
        void operator()(const std::error_code& ec);
      };

      friend struct recv_handler;
      friend struct send_handler;
      
      // Needed to allow changing of state type
      friend struct mono::packet_handler<packet_set, packet::CA_SSO_LOGIN_REQ::login_password>;
      friend struct mono::packet_handler<packet_set, packet::CA_SSO_LOGIN_REQ::token_auth>;
      friend struct mono::packet_handler<packet_set, packet::ATHENA_HA_LOGIN_REQ>;

      /*! Constructs a new session 
        \param server reference to account server
        \param socket pointer to TCP socket
      */
      session(account::state& server_state, std::shared_ptr<asio::ip::tcp::socket> socket);

      /*! Removes this session from server */
      void remove_from_server();
      /*! Defuses ASIO: terminates outstanding operations that depend on the lifetime of this object */
      void defuse_asio();

      /*! Handle disconnect event */
      void on_disconnect();
      /*! Handle open event */
      void on_open();
      /*! Handle before close event */ 
      void before_close();
      /*! Handle conncetion reset event */
      void on_connection_reset();
      /*! Handle EOF event */ 
      void on_eof();
      /*! Handle socket error event */
      void on_socket_error();
      /*! Handle operation aborted event */
      void on_operation_aborted();
      /*! Handle timer inactivity event */
      void on_inactivity_timer();

      /*! Reset inactivity timer */
      void reset_inactivity_timer();

      /*! Returns reference to the state variant corresponding to this session */
      state_variant& variant();

      /*! Returns true if this is a monostate (not initialized as a char server or game client yet) */
      bool is_mono() const;
      /*! Returns reference to state variant interpreted as a monostate */
      mono::state& as_mono();
      /*! Returns true if this is a character server session */ 
      bool is_char_server() const;
      /*! Returns reference to state variant interpreted as a character server state */
      char_server::state& as_char_server();
      /*! Returns true if this is a game client session */
      bool is_client() const;
      /*! Returns reference to state variant interpreted as a game client state */
      client::state& as_client();

      /*! Creates and returns a receive handler */
      recv_handler make_recv_handler();
      /*! Creates and returns a send handler */
      send_handler make_send_handler();
      
    private:
      state_variant session_state_;
    public:
      account::state& server_state_;
    private:
      /*! This session's inactivity threshold time in seconds */
      std::chrono::seconds inactivity_timeout_{120};
    };

    using session_ptr = std::shared_ptr<session>;
  }
}
