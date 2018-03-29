#pragma once

/*! \file handlers.hpp
  Various Asio-compliant handlers for network and timer events
 */

#include <ares/packets>
#include <ares/common/memory>

namespace ares {
  namespace network {
    namespace handler {
      /*! Base class for handlers that hold sessions
       */
      template <typename Session>
      struct session_base {
        session_base(std::shared_ptr<Session> s);
        session_base(const session_base& other);
        session_base(session_base&& other);

      protected:
        void act_on_error(const std::error_code& ec, const std::string& handler_name);

      protected:
        std::shared_ptr<Session> session_;
      };

      /*!
        Handler for reconnect timer for cases when the initiative to reconnect is at our side
       */
      template <typename Session>
      struct reconnect_timer : session_base<Session> {
        reconnect_timer(std::shared_ptr<Session> s,
                        const std::chrono::seconds timeout);
        reconnect_timer(const reconnect_timer& other);
        reconnect_timer(reconnect_timer&& other);        
                              
        void operator()(const std::error_code& ec);

      private:
        std::chrono::seconds timeout_;
      };

      /*!
        Handler to close session after a delay
       */
      template <typename Session>
      struct close_gracefuly_timer : session_base<Session> {
        close_gracefuly_timer(std::shared_ptr<Session> s);
        close_gracefuly_timer(const close_gracefuly_timer& other);
        close_gracefuly_timer(close_gracefuly_timer&& other);

        void operator()(const std::error_code& ec);
      };
      

      /*!
        Handler for network send events
       */
      template <typename Session>
      struct send : session_base<Session> {
        send(std::shared_ptr<Session> s);
        send(const send& other);
        send(send&& other);
      
        void operator()(const std::error_code& ec, const size_t sz);
      };

      /*!
        Handler for receiving packet id
      */
      template <typename Session>
      struct receive_id : session_base<Session> {
        receive_id(std::shared_ptr<Session> s);
        receive_id(const receive_id& other);
        receive_id(receive_id&& other);
      
        void operator()(const std::error_code& ec, const size_t sz);

        friend struct reconnect_timer<Session>;

      private:
        size_t need_more_;
      };

      /*! 
        Handler for receiving packet body after packet id
       */
      template <typename Session>
      struct receive_after_id : session_base<Session> {
        receive_after_id(std::shared_ptr<Session> s,
                         memory::packet_alloc&& ai,
                         const size_t alreay_received);
        receive_after_id(const receive_after_id& other);
        receive_after_id(receive_after_id&& other);

        void operator()(const std::error_code& ec, const size_t sz);
      
      protected:
        memory::packet_alloc ai_;
        size_t bytes_received_;
      };
    }
  }
}
