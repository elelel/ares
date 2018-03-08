#pragma once

#include <atomic>

#include <elelel/network_buffer>

#include <asio/ts/io_context.hpp>
#include <asio/ts/executor.hpp>
#include <asio/ts/socket.hpp>
#include <asio/ts/net.hpp>
#include <asio/ts/buffer.hpp>
#include <asio/ts/internet.hpp>

#include "handlers.hpp"

namespace ares {
  namespace network {
    /*! Generic session for Ares servers
      \tparam Derived concrete session type, must implement: allocate_packet_memory(packet_id), is_dynamic_packet(packet_id)
      \tparam Server server's server type
    */
    template <typename Derived, typename Server>
    struct session {
      session(Server& server,
              const std::optional<asio::ip::tcp::endpoint>& connect_ep,
              std::shared_ptr<asio::ip::tcp::socket> socket,
              const std::chrono::seconds idle_timer_timeout);

      /*! Initiate receive with requesting packet id
       */
      void receive();

      /*! Takes a packet, checks the size and tries to send it as raw data
        \tparam Packet packet type
        \param p const reference to the packet
      */
      template <typename Packet>
      void copy_and_send(const Packet& p);

      /*! Takes raw memory data, copies to send buffer and requests sending if needed
        \param data pointer to raw memory chunk
        \param send_sz number of bytes to send
      */
      void copy_and_send(const void* data, const size_t send_sz);
      
      /*! Constructs packet in-place in free space of send buffer and requests sending if needed
        \tparam Packet packet type
        \tparam Args types of arguments used for packet in-place construction

        \param args arguments used for packet in-place construction
      */
      template <typename Packet, typename... Args>
      void emplace_and_send(Args&&... args);

      /*! Constant reference to send buffer
       */
      const elelel::network_buffer& send_buf() const;
      /*! Reference to the shared pointer for the socket
       */
      std::shared_ptr<asio::ip::tcp::socket>& socket();
      /*! A copy of shared pointer for ASIO io_context
       */
      std::shared_ptr<asio::io_context> io_context() const;

      /*! Reference to server 
       */
      Server& server();
        
      /*! Returns shared pointer to log service
       */
      std::shared_ptr<spdlog::logger> log() const;

      /*! Closes session gracefuly (with a delay)
       */
      void close_gracefuly();

      /*! closes socket immediately and removes session from server */
      void close_abruptly();

      /*! Starts a timer that periodically checks if the session is connected. If not, intiates connection
       */
      void set_reconnect_timer(const std::chrono::seconds first_timeout,
                               const std::chrono::seconds timeout);

      void reset_idle_timer();

      size_t id() const;

    private:
      friend struct handler::reconnect_timer<Derived>;
      friend struct handler::close_gracefuly_timer<Derived>;
      friend struct handler::session_base<Derived>;
      friend struct handler::send<Derived>;
      friend struct handler::receive_id<Derived>;
      friend struct handler::receive_after_id<Derived>;

      template <typename Derived_, typename Packet, typename Server_, typename Session, typename SessionState>
      friend struct packet_handler;
      template <typename Derived_, typename Session_>
      friend struct server;
      
      void send_wake_up(const void* data_start);
      
      /*! Terminates pending Asio operations that might hold reference to this session
       */
      void defuse();

    protected:
      size_t id_;
      elelel::network_buffer send_buf_;
      std::mutex send_mutex_;
      std::mutex recv_mutex_;
      uint16_t packet_id_recv_buf_;

    protected:

      std::shared_ptr<asio::ip::tcp::socket> socket_;
      Server& server_;
      
      std::atomic<bool> connected_;
      std::atomic<bool> connecting_;
      std::atomic<bool> receiving_;
      std::atomic<bool> sending_;

    private:
      const std::optional<asio::ip::tcp::endpoint> connect_ep_;
      std::shared_ptr<asio::steady_timer> idle_timer_;
      std::chrono::seconds idle_timer_timeout_;
      std::shared_ptr<asio::steady_timer> reconnect_timer_;
      std::chrono::seconds reconnect_timer_timeout_;
      std::shared_ptr<asio::steady_timer> close_gracefuly_timer_;
    };
  }
}
