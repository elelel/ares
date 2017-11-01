#pragma once

#include "base.hpp"

namespace ares {
  namespace network {
    namespace handler {
      namespace asio {
        template <typename Handler, typename Session>
        struct receive : base<Handler, Session>  {
          using type = receive<Handler, Session>;

          using base<Handler, Session>::base;

          void operator()(const boost::system::error_code& ec, const size_t received_sz);

        protected:
          elelel::network_buffer& buf();
          std::mutex& mutex();
        };
      }
    }
  }
}
