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

          /*! Handles data send events. If session's buffer still contains unsent data, requests sending it
            \param ec error code
            \param sent_sz number of bytes that were sent */
          void operator()(const boost::system::error_code& ec, size_t sent_sz);

        protected:
          /*! Returns reference to current session's send buffer */
          elelel::network_buffer& buf();
          /*! Returns reference to current session's send mutex */
          std::mutex& mutex();
        };
      }
    }
  }
}
