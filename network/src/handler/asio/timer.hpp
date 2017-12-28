#pragma once

namespace ares {
  namespace network {
    namespace handler {
      namespace asio {
        template <typename Handler, typename Session>
        struct timer : base<Handler, Session> {
          using base<Handler, Session>::base;

          /*! Handles on_timer events from ASIO. Invokes on_success() method of CRTP subclass when timeout event is triggered
           \param ec error code*/
          void operator()(const boost::system::error_code& ec);          
        };
      }
    }
  }
}
