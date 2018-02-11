#pragma once

#include <ares/packets>

namespace ares {
  namespace network {
    namespace handler {
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

      template <typename Session>
      struct send : session_base<Session> {
        send(std::shared_ptr<Session> s);
        send(const send& other);
        send(send&& other);
      
        void operator()(const std::error_code& ec, const size_t sz);
      };

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

      template <typename Session>
      struct receive_after_id : session_base<Session> {
        receive_after_id(std::shared_ptr<Session> s,
                         packet::alloc_info&& ai,
                         const size_t alreay_received);
        receive_after_id(const receive_after_id& other);
        receive_after_id(receive_after_id&& other);

        void operator()(const std::error_code& ec, const size_t sz);
      
      protected:
        packet::alloc_info ai_;
        size_t bytes_received_;
      };
    }
  }
}
