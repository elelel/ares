#pragma once

namespace ares {
  namespace network {
    /*! Base class for ASIO handlers
     */
    template <typename Handler, typename Session>
    struct asio_handler {
      asio_handler(const std::shared_ptr<Session> session);
    protected:
      std::shared_ptr<Session> session_;
      std::shared_ptr<spdlog::logger>& log();
    };
  }
}
