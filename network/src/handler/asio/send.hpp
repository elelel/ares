#pragma once

#include "base.hpp"

namespace ares {
  namespace network {
    namespace handler {
      namespace asio {
        template <typename Handler, typename Session>
        struct send : base<Handler, Session>  {
          using type = send<Handler, Session>;

          using base<Handler, Session>::base;

          void operator()(const boost::system::error_code& ec, size_t sent_sz);

        protected:
          elelel::network_buffer& buf();
          std::mutex& mutex();
        };
      }
    }
  }
}
