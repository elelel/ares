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

          /*! Handles data receive events. If the packet data in receive buffer is incomplete, requests receiving more bytes.
            \param ec error code
            \param received_sz number of bytes received
          */
          void operator()(const std::error_code& ec, const size_t received_sz);

        protected:
          /*! Returns reference to current session's receive buffer */
          elelel::network_buffer& buf();
          /*! Returns reference to current session's receive mutex */
          std::mutex& mutex();
        };
      }
    }
  }
}
