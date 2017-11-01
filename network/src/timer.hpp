#pragma once

#include <boost/asio.hpp>

namespace ares {
  namespace network {
    template <typename Timer, typename Period, typename Session>
    struct timer : handler::asio::base<timer<Timer, Period, Session>, Session> {
      timer(std::shared_ptr<Session> sess, const Period& period);
      timer(const timer<Timer, Period, Session>& other);

      // Interface to asio
      void operator()(const boost::system::error_code& ec);

      void fire();
      void set();
      void cancel();
    protected:
      Period period_;
      std::shared_ptr<boost::asio::steady_timer> timer_;
      std::atomic<bool> executing_;
    };
  }
}
