#pragma once

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
    template <typename Session>
    struct session {
      template <typename Handler, typename Packet, typename Session_, typename State>
      friend struct handler::packet::base;
      template <typename Handler, typename Session_>
      friend struct handler::asio::base;
      template <typename Handler, typename Session_>
      friend struct handler::asio::send;
      template <typename Handler, typename Session_>
      friend struct handler::asio::receive;
      
      session(std::shared_ptr<boost::asio::io_service> io_service,
              std::shared_ptr<spdlog::logger> log,
              std::shared_ptr<boost::asio::ip::tcp::socket> socket);
      ~session();

      void receive(const size_t receive_sz = 0);
      // Takes Packet p, checks size, etc., and invokes send(void*, size_t)
      template <typename Packet>
      void copy_and_send(const Packet& p);
      // Takes raw data, copies to send_buf_ and calls async_write_some if needed
      void copy_and_send(const void* data, const size_t send_sz);
      // Constructs Packet in-place in free space of send_buf_ with Args and calls async_write_some if needed
      template <typename Packet, typename... Args>
      void emplace_and_send(Args&&... args);

      void close_socket();
      std::shared_ptr<boost::asio::ip::tcp::socket>& socket(); 
      std::shared_ptr<boost::asio::io_service> io_service() const;

      bool connected() const;
      std::atomic<bool>& sending();
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
