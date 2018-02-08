#pragma once

namespace ares {
  namespace network {
    template <typename Session>
    struct reconnect_timer_handler {
      reconnect_timer_handler(std::shared_ptr<Session> s,
                              const std::chrono::seconds timeout);
      reconnect_timer_handler(const reconnect_timer_handler& other);
                              
      void operator()(const std::error_code& ec);
    private:
      std::shared_ptr<Session> s_;
      std::chrono::seconds timeout_;
    };
    
    template <typename Session>
    struct send_handler {
      send_handler(std::shared_ptr<Session> s);
      send_handler(const send_handler& other);
      
      void operator()(const std::error_code& ec, const size_t sz);
    private:
      std::shared_ptr<Session> s_;
    };

    template <typename Session>
    struct receive_handler {
      receive_handler(std::shared_ptr<Session> s);
      receive_handler(const receive_handler& other);
      
      void operator()(const std::error_code& ec, const size_t sz);
    private:
      std::shared_ptr<Session> s_;
    };
  }
}
