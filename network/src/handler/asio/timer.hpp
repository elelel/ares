#pragma once

namespace ares {
  namespace network {
    namespace handler {
      namespace asio {
        template <typename Handler, typename Session>
        struct timer : base<Handler, Session> {
          using base<Handler, Session>::base;

          void operator()(const boost::system::error_code& ec);          
        };
      }
    }
  }
}
