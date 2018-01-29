#pragma once

/*! \file session.hpp
\brief Generic Ares server session

*/

#include <atomic>

#include <boost/asio.hpp>
#include <boost/asio/steady_timer.hpp>
#include <spdlog/spdlog.h>
#include <elelel/network_buffer>

#include "handler/packet/base.hpp"
#include "handler/asio/send.hpp"
#include "handler/asio/receive.hpp"

namespace ares {
  namespace network {
    /*! Generic session for Ares servers
      \tparam Session concrete session type
    */
    template <typename Session>
    struct session {
      template <typename Handler, typename Packet, typename Server, typename Session_, typename State>
      friend struct handler::packet::base;
      template <typename Handler, typename Session_>
      friend struct handler::asio::base;
      template <typename Handler, typename Session_>
      friend struct handler::asio::send;
      template <typename Handler, typename Session_>
      friend struct handler::asio::receive;

      /*! Constructs an Ares session
        \param io_service pointer to ASIO io_service
        \param log pointer to a logger
        \param socket pointer to session's network socket 
      */
      session(std::shared_ptr<boost::asio::io_service> io_service,
              std::shared_ptr<spdlog::logger> log,
              std::shared_ptr<boost::asio::ip::tcp::socket> socket);
      ~session();

      /*! Receives bytes from socket into receive buffer
        \param receive_sz number of bytes to request. If 0, requests maximum unfragmented buffer size
      */
      void receive(const size_t receive_sz = 0);
      
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
      
      /*! Constructs Packet in-place in free space of send buffer and requests sending if needed
        \tparam Packet packet type
        \tparam Args types of arguments used for packet in-place construction

        \param args arguments used for packet in-place construction
      */
      template <typename Packet, typename... Args>
      void emplace_and_send(Args&&... args);

      /*! Closes the socket gracefully
       */
      void close_socket();

      /*! Returns reference to the shared pointer for the socket
       */
      std::shared_ptr<boost::asio::ip::tcp::socket>& socket();
      /*! Returns a copy of shared pointer for ASIO io_service
       */
      std::shared_ptr<boost::asio::io_service> io_service() const;

      /*! Check if the session is connected
       */
      bool connected() const;

      /*! Check if the session is in process of sending
       */
      std::atomic<bool>& sending();

      /*! Set connected state flag to true
       */
      void set_connected();
    protected:
      std::shared_ptr<boost::asio::io_service> io_service_;
      std::shared_ptr<spdlog::logger> log_;
      std::shared_ptr<boost::asio::ip::tcp::socket> socket_;
      std::atomic<bool> connected_;

      boost::asio::steady_timer inactivity_timer_;
      std::mutex recv_mutex_;
      elelel::network_buffer recv_buf_;
      std::mutex send_mutex_;
      elelel::network_buffer send_buf_;

      std::atomic<bool> receiving_;
      std::atomic<bool> sending_;

    private:
      void send_wake_up(const void* data_start);
    };
  }
}
