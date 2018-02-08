#pragma once

namespace ares {
  namespace network {
    template <typename Handler, typename Session>
    struct receive_handler : asio_handler {
      using asio_handler<Handler, Session>::base;

      /*! Handles data receive events.
        \param ec error code
        \param received_sz number of bytes received
      */
      void operator()(const std::error_code& ec, const size_t received_sz);
    protected:
      /*! Returns reference to current session's receive buffer */
      elelel::network_buffer& buf();
      /*! Returns reference to current session's receive busy flag */
      std::atomic<bool>& buf_busy();
      
    };
  }
}
