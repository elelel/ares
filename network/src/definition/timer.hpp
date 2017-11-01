#include "../timer.hpp"

template <typename Timer, typename Period, typename Session>
ares::network::timer<Timer, Period, Session>::timer(std::shared_ptr<Session> sess,
                                             const Period& period) :
  handler::asio::base<timer<Timer, Period, Session>, Session>(sess),
  period_(period),
  timer_(std::make_shared<boost::asio::steady_timer>(*sess->io_service())),
  executing_(false) {
  }

template <typename Timer, typename Period, typename Session>
ares::network::timer<Timer, Period, Session>::timer(const timer<Timer, Period, Session>& other) :
  handler::asio::base<timer<Timer, Period, Session>, Session>(other),
  period_(other.period_),
  timer_(other.timer_),
  executing_(other.executing_.load()) {
}

template <typename Timer, typename Period, typename Session>
inline void ares::network::timer<Timer, Period, Session>::operator()(const boost::system::error_code& ec) {
  if (!executing_) {
    executing_ = true;
    switch (ec.value()) {
    case 0:
      static_cast<Timer&>(*this).on_timer();
      break;
    case boost::asio::error::operation_aborted:
      SPDLOG_TRACE(this->log(), "{} timer operation aborted", static_cast<Timer&>(*this).name());
      break;
    default:
      this->log()->error("{} timer error {}, {}", static_cast<Timer&>(*this).name(), ec.value(), ec.message());
      throw std::runtime_error("timer error");
    }
    executing_ = false;
  }
}

template <typename Timer, typename Period, typename Session>
inline void ares::network::timer<Timer, Period, Session>::fire() {
  cancel();
  timer_->expires_at(std::chrono::steady_clock::now());
  timer_->async_wait(*this);
}

template <typename Timer, typename Period, typename Session>
inline void ares::network::timer<Timer, Period, Session>::set() {
  cancel();
  timer_->expires_at(std::chrono::steady_clock::now() + period_);
  timer_->async_wait(*this);
}

template <typename Timer, typename Period, typename Session>
inline void ares::network::timer<Timer, Period, Session>::cancel() {
  timer_->cancel();
}
