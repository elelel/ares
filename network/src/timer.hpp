#pragma once

#include <asio.hpp>

/*! \file timer.hpp
  \brief ASIO timer abstraction

*/

namespace ares {
  namespace network {
    /*! Timer abstraction for Ares servers
      \tparam Timer concrete timer type
      \tparam Period time for the timeout period
      \tparam Session concrete session type this timer is assiociated with
    */
    template <typename Timer, typename Period, typename Session>
    struct timer : handler::asio::base<timer<Timer, Period, Session>, Session> {
      timer(std::shared_ptr<Session> sess, const Period& period);
      timer(const timer<Timer, Period, Session>& other);

      // Interface to asio
      void operator()(const std::error_code& ec);

      void fire();
      void set();
      void cancel();
    protected:
      Period period_;
      std::shared_ptr<asio::steady_timer> timer_;
      std::atomic<bool> executing_;
    };
  }
}
